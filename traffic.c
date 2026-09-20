#include <stdio.h>
#include <string.h>

#define N 13

typedef struct {
    int id;
    char name[10];
    int degree;
} Route;

Route nodes[N] = {
    {0, "A->B", 0},
    {1, "A->C", 0},
    {2, "A->D", 0},
    {3, "B->A", 0},
    {4, "B->C", 0},
    {5, "B->D", 0},
    {6, "D->A", 0},
    {7, "D->B", 0},
    {8, "D->C", 0},
    {9, "E->A", 0},
    {10, "E->B", 0},
    {11, "E->C", 0},
    {12, "E->D", 0}
};

int adj[N][N] = {0};

void addConflict(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void buildGraph() {
    // 0:AB, 1:AC, 2:AD
    addConflict(0, 4); addConflict(0, 5); addConflict(0, 6); addConflict(0, 7); addConflict(0, 9); addConflict(0, 10);
    addConflict(1, 4); addConflict(1, 5); addConflict(1, 6); addConflict(1, 7); addConflict(1, 9); addConflict(1, 10); addConflict(1, 11);
    addConflict(2, 4); addConflict(2, 5); addConflict(2, 6); addConflict(2, 7); addConflict(2, 9); addConflict(2, 10); addConflict(2, 11);

    // 4:BC, 5:BD
    addConflict(4, 6); addConflict(4, 7); addConflict(4, 10); addConflict(4, 11);
    addConflict(5, 6); addConflict(5, 7); addConflict(5, 10); addConflict(5, 11);

    // 9:EA
    addConflict(9, 6); addConflict(9, 7); addConflict(9, 10); addConflict(9, 11);

    // 6:DA, 7:DB
    addConflict(6, 10); addConflict(6, 11);
    addConflict(7, 10); addConflict(7, 11);
}

void calculateDegrees() {
    for (int i = 0; i < N; i++) {
        nodes[i].degree = 0;
        for (int j = 0; j < N; j++) {
            if (adj[i][j]) {
                nodes[i].degree++;
            }
        }
    }
}

// 冒泡排序：按degree从大到小排序数组sortedNodes
void sortNodes(int sortedNodes[]) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            int a = sortedNodes[j];
            int b = sortedNodes[j+1];
            if (nodes[a].degree < nodes[b].degree) {
                int tmp = sortedNodes[j];
                sortedNodes[j] = sortedNodes[j+1];
                sortedNodes[j+1] = tmp;
            }
        }
    }
}

// 判断node染color是否安全
int isSafe(int node, int color, int colors[]) {
    for (int i = 0; i < N; i++) {
        if (adj[node][i] && colors[i] == color) {
            return 0;
        }
    }
    return 1;
}

// ==========新增校验函数==========
void checkResult(int colors[])
{
    int valid = 1;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(adj[i][j] && colors[i] == colors[j])
            {
                printf("\n[警告]冲突路线 %s 与 %s 被分到同一相位！\n", nodes[i].name, nodes[j].name);
                valid = 0;
            }
        }
    }
    if(valid)
        printf("\n>>>> 校验完成：所有冲突路线相位分配合法 <<<<\n");
    else
        printf("\n>>>> 校验完成：发现分配冲突 <<<<\n");
}

// 回溯函数
int solve(int idx, int remainingNodes[], int remLen, int numColors, int colors[]) {
    if (idx == remLen) {
        return 1;
    }
    int node = remainingNodes[idx];
    for (int c = 1; c <= numColors; c++) {
        if (isSafe(node, c, colors)) {
            colors[node] = c;
            if (solve(idx + 1, remainingNodes, remLen, numColors, colors)) {
                return 1;
            }
            colors[node] = -1;
        }
    }
    return 0;
}

int main(void) {
    printf("========== 多岔路口交通管理 - 求解过程 ==========\n");
    printf("步骤1：构建图模型。将13条路线抽象为节点，冲突关系抽象为边。\n");
    buildGraph();

    printf("步骤2：计算每个节点的冲突度数，并按度数从大到小排序。\n");
    calculateDegrees();

    int sortedNodes[N];
    for (int i = 0; i < N; i++) {
        sortedNodes[i] = i;
    }
    sortNodes(sortedNodes);

    printf("排序后的节点处理顺序（优先处理冲突最多的节点）: ");
    for (int i = 0; i < N; i++) {
        printf("%s ", nodes[sortedNodes[i]].name);
    }
    printf("\n\n");

    printf("步骤3：开始图着色搜索（采用回溯法寻找最优解）。\n");

    int colors[N];
    int remainingNodes[N];
    int remLen = 0;
    int found = 0;

    for (int numColors = 1; numColors <= 4; numColors++) {
        // 重置颜色数组
        for (int i = 0; i < N; i++) {
            colors[i] = -1;
        }
        colors[3] = 0;
        colors[8] = 0;
        colors[12] = 0;

        remLen = 0;
        for (int i = 0; i < N; i++) {
            int n = sortedNodes[i];
            if (colors[n] == -1) {
                remainingNodes[remLen++] = n;
            }
        }

        if (solve(0, remainingNodes, remLen, numColors, colors)) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("步骤4：搜索完成！成功找到最少相位分配。\n\n");
        printf("========== 最终相位分配结果 ==========\n");
        for (int c = 0; c <= 4; c++) {
            if (c == 0) {
                printf("相位 0 (0号色灯 / 常通相位，右转车辆无冲突，常亮):\n");
            } else {
                printf("相位 %d (%d号色灯 / 轮转相位):\n", c, c);
            }
            for (int i = 0; i < N; i++) {
                if (colors[i] == c) {
                    printf("  - %s\n", nodes[i].name);
                }
            }
            printf("-------------------------------------------\n");
        }
        // 输出完结果调用校验
        checkResult(colors);
    } else {
        printf("步骤4：搜索失败，未能找到合适的相位分配。\n");
    }
    return 0;
}
