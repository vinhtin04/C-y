#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE_MAX 100
#define MAX_CHILD 10  // Số lượng mục con tối đa

typedef struct Node {
    char type[20];               // Loại mục ('chapter', 'section', ...)
    char title[TITLE_MAX];       // Tên mục
    int pages;                   // Số trang
    struct Node* children[MAX_CHILD]; // Mảng tĩnh chứa các mục con
    int child_count;             // Số mục con
} Node;

// Tạo node mới
Node* create_node(const char* type, const char* title, int pages) {
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->type, type);
    strcpy(n->title, title);
    n->pages = pages;
    n->child_count = 0;
    for (int i = 0; i < MAX_CHILD; i++) {
        n->children[i] = NULL;
    }
    return n;
}

// Thêm mục con
int add_child(Node* parent, Node* child) {
    if (parent->child_count >= MAX_CHILD) {
        printf("Không thể thêm con vào '%s': Quá giới hạn\n", parent->title);
        return 0;
    }
    parent->children[parent->child_count++] = child;
    return 1;
}

// Đếm số chương
int count_chapters(Node* root) {
    if (strcmp(root->type, "chapter") != 0) return 0;

    int count = 1;
    for (int i = 0; i < root->child_count; i++) {
        count += count_chapters(root->children[i]);
    }
    return count;
}

// Tìm chương dài nhất
Node* find_longest(Node* root, int* max_pages) {
    Node* longest = NULL;
    if (strcmp(root->type, "chapter") == 0 && root->pages > *max_pages) {
        *max_pages = root->pages;
        longest = root;
    }

    for (int i = 0; i < root->child_count; i++) {
        Node* candidate = find_longest(root->children[i], max_pages);
        if (candidate != NULL) {
            longest = candidate;
        }
    }
    return longest;
}

// Xóa mục
int remove_node(Node* parent, const char* title) {
    for (int i = 0; i < parent->child_count; i++) {
        if (strcmp(parent->children[i]->title, title) == 0) {
            free(parent->children[i]);

            for (int j = i; j < parent->child_count - 1; j++) {
                parent->children[j] = parent->children[j + 1];
            }

            parent->child_count--;
            parent->children[parent->child_count] = NULL;
            return 1;
        }

        if (remove_node(parent->children[i], title)) {
            return 1;
        }
    }
    return 0;
}

// Cập nhật số trang
int update_pages(Node* root) {
    int total = root->pages;
    for (int i = 0; i < root->child_count; i++) {
        total += update_pages(root->children[i]);
    }
    root->pages = total;
    return total;
}

// Giải phóng bộ nhớ cây
void free_tree(Node* root) {
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }
    free(root);
}

// Hàm chính
int main() {
    // Tạo cây sách
    Node* root = create_node("book", "Embedded Systems", 0);

    Node* chap1 = create_node("chapter", "Introduction", 10);
    Node* chap2 = create_node("chapter", "Advanced Topics", 20);

    add_child(root, chap1);
    add_child(root, chap2);

    Node* sec1 = create_node("section", "Basic Concepts", 5);
    add_child(chap1, sec1);

    // Đếm số chương
    printf("So chuong: %d\n", count_chapters(root));

    // Tìm chương dài nhất
    int max_pages = 0;
    Node* longest = find_longest(root, &max_pages);
    if (longest) {
        printf("Chuong dai nhay: %s (%d trang)\n", longest->title, longest->pages);
    }

    // Xóa mục
    if (remove_node(root, "Basic Concepts")) {
        printf("Da xoa'Basic Concepts'\n");
    }

    // Cập nhật số trang
    update_pages(root);
    printf("Tong so trang: %d\n", root->pages);

    // Giải phóng bộ nhớ
    free_tree(root);

    return 0;
}
