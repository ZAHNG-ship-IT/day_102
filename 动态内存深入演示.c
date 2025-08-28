#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
  #include <malloc.h>
  #define USABLE_SIZE(p) _msize(p)
#elif defined(__GLIBC__)
  #include <malloc.h>
  #define USABLE_SIZE(p) malloc_usable_size(p)
#else
  #define USABLE_SIZE(p) ((size_t)0)
#endif

struct Node {
    int id;
    char name[32];
    double value;
    struct Node* next;
};

static void print_block(const char* tag, void* p, size_t req) {
    size_t usable = USABLE_SIZE(p);
    uintptr_t addr = (uintptr_t)p;
    printf("%-18s req=%4zuB  ptr=%p  usable=%zuB  align16=%s  align8=%s\n",
           tag, req, p,
           usable,
           (addr % 16 == 0 ? "Y" : "N"),
           (addr % 8 == 0 ? "Y" : "N"));
}

int main(void) {
    printf("=== 动态内存深入演示 (heap/对齐/可用大小/碎片化) ===\n\n");

    // 1) 基础：不同大小的malloc，观察地址对齐与实际可用大小
    size_t sizes[] = {1, 8, 13, 33, 64, 65, 200, 1000};
    const int N = (int)(sizeof(sizes)/sizeof(sizes[0]));
    void* blocks[16] = {0};

    printf("[1] malloc 不同大小块\n");
    for (int i = 0; i < N; ++i) {
        blocks[i] = malloc(sizes[i]);
        print_block("malloc", blocks[i], sizes[i]);
    }
    printf("\n备注: usable=分配器实际分给你的块大小(含对齐/桶粒度); 对齐一般是16/8字节。\n\n");

    // 2) 节点：每个节点内部数据连续，但不同节点之间不保证连续
    printf("[2] 链表节点分配 (struct Node)\n");
    printf("sizeof(struct Node) = %zu\n", sizeof(struct Node));

    struct Node* n1 = (struct Node*)malloc(sizeof(struct Node));
    struct Node* n2 = (struct Node*)malloc(sizeof(struct Node));
    struct Node* n3 = (struct Node*)malloc(sizeof(struct Node));
    n1->next = n2; n2->next = n3; n3->next = NULL;

    print_block("Node n1", n1, sizeof(struct Node));
    print_block("Node n2", n2, sizeof(struct Node));
    print_block("Node n3", n3, sizeof(struct Node));

    // 节点内部字段地址：连续且按各自类型对齐
    printf("\n节点内部字段地址(证明节点内部是连续内存):\n");
    printf("  &n1->id      = %p\n", (void*)&n1->id);
    printf("  &n1->name    = %p\n", (void*)n1->name);
    printf("  &n1->value   = %p\n", (void*)&n1->value);
    printf("  &n1->next    = %p\n", (void*)&n1->next);

    // 不同节点之间的地址差
    printf("\n不同节点之间的地址差(不代表大小限制, 仅表示在堆上的相对位置):\n");
    printf("  n2 - n1 = %lld bytes\n", (long long)((char*)n2 - (char*)n1));
    printf("  n3 - n2 = %lld bytes\n", (long long)((char*)n3 - (char*)n2));

    // 3) 简单碎片化演示：释放中间块，再申请不同大小的块，看回收利用情况
    printf("\n[3] 碎片化与重用演示\n");
    void* A = malloc(1024);
    void* B = malloc(1024);
    void* C = malloc(1024);
    print_block("A(1024)", A, 1024);
    print_block("B(1024)", B, 1024);
    print_block("C(1024)", C, 1024);

    printf("-- 释放B --\n");
    free(B);

    void* D = malloc(512);   // 可能重用B留下的空洞
    void* E = malloc(900);   // 看是否能放入剩余碎片或新区域
    print_block("D(512)", D, 512);
    print_block("E(900)", E, 900);

    // 清理
    for (int i = 0; i < N; ++i) free(blocks[i]);
    free(n1); free(n2); free(n3);
    free(A); free(C); free(D); free(E);

    printf("\n结论: malloc返回的是一块\"内部连续\"的内存; 不同块之间地址无序、彼此不连续。\n");
    return 0;
}
