#include <stdio.h>
#include <stdbool.h>

#define N_ROUTE 13   // 总共13条通行路线

/**
 * @brief 判断路线v分配颜色c是否安全
 * adj：邻接矩阵；v当前路线；color数组；c待分配相位
 */
bool isSafe(int adj[N_ROUTE][N_ROUTE], int v, int color[], int c)
{
    for (int i = 0; i < N_ROUTE; i++)
    {
        if (adj[v][i] && color[i] == c)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief 回溯图着色
 */
bool graphColoringHelper(int adj[N_ROUTE][N_ROUTE], int v, int color[], int totalPhase)
{
    if (v == N_ROUTE)
    {
        return true;
    }
    for (int c = 1; c <= totalPhase; c++)
    {
        if (isSafe(adj, v, color, c))
        {
            color[v] = c;
            if (graphColoringHelper(adj, v + 1, color, totalPhase))
            {
                return true;
            }
            color[v] = 0;
        }
    }
    return false;
}

/**
 * @brief 寻找最小相位分配
 */
bool graphColoring(int adj[N_ROUTE][N_ROUTE], int color[])
{
    // 从4个相位开始尝试
    for (int phase = 4; phase <= N_ROUTE; phase++)
    {
        for (int i = 0; i < N_ROUTE; i++)
            color[i] = 0;
        if (graphColoringHelper(adj, 0, color, phase))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 校验函数：检查结果是否存在冲突路线同相位
 */
bool checkResult(int adj[N_ROUTE][N_ROUTE], int color[])
{
    for (int i = 0; i < N_ROUTE; i++)
    {
        for (int j = 0; j < N_ROUTE; j++)
        {
            if (adj[i][j] && color[i] == color[j])
            {
                printf("【校验错误】路线 %d 和路线 %d 冲突，被分到同一相位！\n", i, j);
                return false;
            }
        }
    }
    return true;
}

int main(void)
{
    // 初始化邻接矩阵全部置0
    int adj[N_ROUTE][N_ROUTE] = {0};

    // =====完整冲突邻接矩阵=====
    // 题目示例：E→B(7) 和 A→D(2)冲突
    adj[7][2] = 1; adj[2][7] = 1;
    //A出发路线冲突
    adj[0][3] = 1; adj[3][0] = 1; //A→B 与 A→E冲突
    adj[0][6] = 1; adj[6][0] = 1; //A→B 与 B→D冲突
    adj[1][7] = 1; adj[7][1] = 1; //A→C 与 B→E冲突
    adj[2][4] = 1; adj[4][2] = 1; //A→D 与 B→A冲突
    adj[2][7] = 1; adj[7][2] = 1; //A→D 与 B→E冲突
    adj[3][5] = 1; adj[5][3] = 1; //A→E 与 B→C冲突
    //B出发路线冲突
    adj[4][10] = 1; adj[10][4] = 1; //B→A 与 C→D冲突
    adj[5][8] = 1; adj[8][5] = 1; //B→C 与 C→A冲突
    adj[6][11] = 1; adj[11][6] = 1; //B→D 与 C→E冲突
    adj[7][8] = 1; adj[8][7] = 1; //B→E 与 C→A冲突
    //C出发路线冲突
    adj[8][11] = 1; adj[11][8] = 1; //C→A 与 C→E冲突
    adj[9][10] = 1; adj[10][9] = 1; //C→B 与 C→D冲突
    //E→C（索引12）
    adj[12][4] = 1; adj[4][12] = 1; //E→C 和 B→A冲突
    adj[12][6] = 1; adj[6][12] = 1; //E→C 和 B→D冲突

    int color[N_ROUTE];
    graphColoring(adj, color);

    printf("========五岔路口交通信号灯相位调度结果========\n");
    for(int phase = 0; phase < 20; phase++)
    {
        bool hasRoute = false;
        printf("【相位%d】: ", phase+1);
        for(int i=0; i<N_ROUTE; i++)
        {
            if(color[i] == phase+1)
            {
                printf("%d ",i);
                hasRoute = true;
            }
        }
        if(hasRoute)
            printf("\n");
    }

    // 执行结果校验
    printf("\n====================方案校验====================\n");
    if(checkResult(adj, color))
    {
        printf("✅ 校验通过，所有冲突路线相位不同，调度方案合法！\n");
    }
    else
    {
        printf("❌ 校验失败，方案存在冲突！\n");
    }
    printf("\n说明：同一个相位内的路线可以同时放行绿灯，不会产生碰撞。\n");
    return 0;
}