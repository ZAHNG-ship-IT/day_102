#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 小节点：只有基本数据
struct SmallNode {
    int data;
    struct SmallNode* next;
};

// 中等节点：包含更多数据
struct MediumNode {
    int data;
    char name[50];
    double value;
    struct MediumNode* next;
};

// 大节点：包含大量数据
struct LargeNode {
    int data;
    char description[200];
    int array[50];        // 200字节的数组
    double values[20];    // 160字节的double数组
    struct LargeNode* next;
};

int main() {
    printf("=== 不同大小节点的内存分配演示 ===\n\n");
    
    // 1. 小节点测试
    printf("1. 小节点测试 (sizeof = %zu 字节):\n", sizeof(struct SmallNode));
    struct SmallNode* small_nodes[3];
    for(int i = 0; i < 3; i++) {
        small_nodes[i] = malloc(sizeof(struct SmallNode));
        small_nodes[i]->data = i;
        printf("  小节点%d地址: %p\n", i+1, (void*)small_nodes[i]);
    }
    
    // 计算小节点地址差
    for(int i = 1; i < 3; i++) {
        long long diff = (char*)small_nodes[i] - (char*)small_nodes[i-1];
        printf("  地址差: %lld 字节\n", diff);
    }
    
    printf("\n");
    
    // 2. 中等节点测试
    printf("2. 中等节点测试 (sizeof = %zu 字节):\n", sizeof(struct MediumNode));
    struct MediumNode* medium_nodes[3];
    for(int i = 0; i < 3; i++) {
        medium_nodes[i] = malloc(sizeof(struct MediumNode));
        medium_nodes[i]->data = i;
        printf("  中节点%d地址: %p\n", i+1, (void*)medium_nodes[i]);
    }
    
    // 计算中等节点地址差
    for(int i = 1; i < 3; i++) {
        long long diff = (char*)medium_nodes[i] - (char*)medium_nodes[i-1];
        printf("  地址差: %lld 字节\n", diff);
    }
    
    printf("\n");
    
    // 3. 大节点测试
    printf("3. 大节点测试 (sizeof = %zu 字节):\n", sizeof(struct LargeNode));
    struct LargeNode* large_nodes[3];
    for(int i = 0; i < 3; i++) {
        large_nodes[i] = malloc(sizeof(struct LargeNode));
        large_nodes[i]->data = i;
        printf("  大节点%d地址: %p\n", i+1, (void*)large_nodes[i]);
    }
    
    // 计算大节点地址差
    for(int i = 1; i < 3; i++) {
        long long diff = (char*)large_nodes[i] - (char*)large_nodes[i-1];
        printf("  地址差: %lld 字节\n", diff);
    }
    
    printf("\n=== 内存使用验证 ===\n");
    
    // 验证大节点确实可以存储大量数据
    printf("验证大节点可以存储大量数据:\n");
    strcpy(large_nodes[0]->description, "这是一个很长的描述文本，用来测试大节点是否能正常存储大量数据而不会出现内存冲突问题。");
    
    // 填充数组
    for(int i = 0; i < 50; i++) {
        large_nodes[0]->array[i] = i * i;
    }
    for(int i = 0; i < 20; i++) {
        large_nodes[0]->values[i] = i * 3.14159;
    }
    
    printf("大节点0的描述: %.50s...\n", large_nodes[0]->description);
    printf("大节点0的数组前5个元素: ");
    for(int i = 0; i < 5; i++) {
        printf("%d ", large_nodes[0]->array[i]);
    }
    printf("\n");
    printf("大节点0的values前3个元素: ");
    for(int i = 0; i < 3; i++) {
        printf("%.2f ", large_nodes[0]->values[i]);
    }
    printf("\n");
    
    // 释放内存
    for(int i = 0; i < 3; i++) {
        free(small_nodes[i]);
        free(medium_nodes[i]);
        free(large_nodes[i]);
    }
    
    return 0;
}
