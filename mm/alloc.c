/*************************************************************************//**
 *****************************************************************************
 * @file   alloc.c
 * @brief  
 * @author Shuwen Chen
 * @date   The Dec  6 10:13:15 2021
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"



int parent(int i)
{
    return (i - 1) / 2;
}

int child_left(int i)
{
    return 2 * i + 1;
}

int child_right(int i)
{
    return 2 * i + 2;
}

int brother(int i)
{
    if(i % 2 == 0)
        return i - 1;
    else
        return i + 1;
}

int pow2(int exp)
{
    int res = 1;
    while(exp)
    {
        res <<= 1;
        exp--;
    }
    return res;
}

// 当前划分层所包含的页数目
int pg_cnt(int depth)
{
    return PAGE_CNT / pow2(depth);
}

// 结点的基地址
u32 base_addr(int i, int depth)
{
    if(i == 0)
    {
        return ALLOC_BASE;
    }

    int cur_pg_cnt = pg_cnt(depth);
    if(i % 2 == 1)
    {
        return base_addr(parent(i), depth - 1);
    }
    else
    {
        return base_addr(parent(i), depth - 1) + cur_pg_cnt * PAGE_SIZE;
    }
}

// 结点的基页号
int base_pg(int i, int depth)
{
    if(i == 0)
        return 0;

    int cur_pg_cnt = pg_cnt(depth);
    if(i % 2 == 1)
        return base_pg(parent(i), depth - 1);
    else
        return base_pg(parent(i), depth - 1) + cur_pg_cnt;
}

// 输出二叉树
void output(int root, int depth)
{
    if(root >= NODE_CNT)
        return;
    if(nodes[root] == BD_NONE)
        return;

    switch(nodes[root])
    {
        case BD_NONE:
            printl("%d:\nsize:0x%x addr:0x%x stat:NONE\n", root, pg_cnt(depth) * PAGE_SIZE, base_addr(root, depth));
            break;
        case BD_FREE:
            printl("%d:\nsize:0x%x addr:0x%x stat: FREE\n", root, pg_cnt(depth) * PAGE_SIZE, base_addr(root, depth));
            break;
        case BD_SPLIT:
            printl("%d:\nsize:0x%x addr:0x%x stat: SPLIT\n", root, pg_cnt(depth) * PAGE_SIZE, base_addr(root, depth));
            break;
        case BD_OCCUPIED:
            printl("%d:\nsize:0x%x addr:0x%x stat: OCCUPIED\n", root, pg_cnt(depth) * PAGE_SIZE, base_addr(root, depth));
            break;
        default:
            printl("%d:\nsize:0x%x addr:0x%x stat: ERROR\n", root, pg_cnt(depth) * PAGE_SIZE, base_addr(root, depth));
    }

    output(child_left(root), depth + 1);
    output(child_right(root), depth + 1);
    return;
}

/**
 * 递归分配内存
 * root: 当前根结点编号
 * depth: 当前结点深度
 * size: 所需分配大小
 * return: 分配地址，如果不够分配返回None
*/
u32 tree_alloc(int root, int depth, u32 size)
{
    int cur_pg_cnt = pg_cnt(depth);
    int child_pg_cnt =  cur_pg_cnt / 2;
    int cur_size =  cur_pg_cnt * PAGE_SIZE;
    int child_size = child_pg_cnt * PAGE_SIZE;

    if(root == 0 && size > cur_size)
        return NULL;

    if(nodes[root] == BD_OCCUPIED)
        return NULL;

    else if(nodes[root] == BD_FREE)
    {
        if(size > child_size)
        {
            nodes[root] = BD_OCCUPIED;
            return base_addr(root, depth);
        }
        else if(size <= child_size)
        {
            nodes[root] = BD_SPLIT;
            nodes[child_left(root)] = BD_FREE;
            nodes[child_right(root)] = BD_FREE;
            return tree_alloc(child_left(root), depth + 1, size);
        }
    }

    else if(nodes[root] == BD_SPLIT)
    {
        if(size > child_size)
            return NULL;
        else if(size <= child_size)
        {
            u32 laddr = tree_alloc(child_left(root), depth + 1, size);
            if(laddr != NULL)
                return laddr;
            else
            {
                u32 raddr = tree_alloc(child_right(root), depth + 1, size);
                if(raddr != NULL)
                    return raddr;
                else
                    return NULL;
            }
        }
    }
}

// mm/alloc.c
PUBLIC int do_alloc()
{
    printl("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printl("Alloc 0x%x byte(s):\n", mm_msg.MM_SIZE);

    mm_msg.MM_ADDR = tree_alloc(0, 0, mm_msg.MM_SIZE);

    printl("Alloc at: 0x%x\n", mm_msg.MM_ADDR);

    printl("Alloc Status:\n");
    output(0, 0);
    printl("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    return 0;
}

/**
 * 递归释放内存
 * root: 当前根结点编号
 * depth: 当前结点深度
 * addr: 内存的基地址
*/
int tree_free(int root, int depth, u32 addr)
{
        if(addr == NULL)
            return FREE_FAILURE;

        u32 rt_addr_low = base_addr(root, depth);
        u32 rt_addr_high = rt_addr_low + pg_cnt(depth) * PAGE_SIZE;

        if(addr < rt_addr_low || addr >= rt_addr_high)
            return FREE_FAILURE;

        if(nodes[root] == BD_OCCUPIED)
        {
            if(rt_addr_low == addr)
            {
                nodes[root] = BD_FREE;
                return FREE_SUCCESS;
            }
            else
                return FREE_FAILURE;
        }

        else if(nodes[root] == BD_SPLIT)
        {
            u32 l_addr = base_addr(child_left(root), depth + 1);
            u32 r_addr = base_addr(child_right(root), depth + 1);

            if(addr >= l_addr && addr < r_addr)
            {
                int stat = tree_free(child_left(root), depth + 1, addr);
                if(stat == FREE_FAILURE)return stat;

                if(nodes[child_left(root)] == BD_FREE)
                {
                    if(nodes[child_right(root)] == BD_FREE)
                    {
                        nodes[root] = BD_FREE;
                        nodes[child_left(root)] = BD_NONE;
                        nodes[child_right(root)] = BD_NONE;
                        return FREE_SUCCESS;
                    }
                    else
                        return FREE_SUCCESS;
                }
                else if(nodes[child_left(root)] == BD_SPLIT)
                {
                    return FREE_SUCCESS;
                }
                else
                    return FREE_FAILURE;
            }
            else if(addr >= r_addr)
            {
                int stat = tree_free(child_right(root), depth + 1, addr);
                if(stat == FREE_FAILURE)return stat;

                if(nodes[child_right(root)] == BD_FREE)
                {
                    if(nodes[child_left(root)] == BD_FREE)
                    {
                        nodes[root] = BD_FREE;
                        nodes[child_left(root)] = BD_NONE;
                        nodes[child_right(root)] = BD_NONE;
                        return FREE_SUCCESS;
                    }
                    else
                        return FREE_SUCCESS;
                }
                else if(nodes[child_right(root)] == BD_SPLIT)
                {
                    return FREE_SUCCESS;
                }
                else
                    return FREE_FAILURE;                
            }
            else
                return FREE_FAILURE;
        }
        else
            return FREE_FAILURE;
}


// mm/alloc.c
PUBLIC int do_free()
{
    printl("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printl("Free at: 0x%x\n", mm_msg.MM_ADDR);

    int stat = tree_free(0, 0, mm_msg.MM_ADDR);
    if(stat == FREE_SUCCESS)
    {
        printl("Free Success!\n");
        printl("Free Status:\n");
        output(0, 0);
    }
    else
    {
        printl("Free Failure!\n");
    }

    printl("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    return stat;
}
