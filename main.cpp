#include <iostream>
#include <fstream>
using namespace std;

// ---------------- Queue (linked-list queue for runs) ----------------
struct RunNode {
    unsigned char c;
    unsigned char v;
    RunNode* next;
    RunNode(unsigned char cc, unsigned char vv) : c(cc), v(vv), next(NULL) {}
};

struct RunQueue {
    RunNode* head;
    RunNode* tail;
    RunQueue() : head(NULL), tail(NULL) {}

    void push(unsigned char c, unsigned char v) {
        RunNode* n = new RunNode(c, v);
        if (!tail) head = tail = n;
        else { tail->next = n; tail = n; }
    }

    bool pop(unsigned char &c, unsigned char &v) {
        if (!head) return false;
        RunNode* n = head;
        c = n->c; v = n->v;
        head = head->next;
        if (!head) tail = NULL;
        delete n;
        return true;
    }
};

// ---------------- Min-Heap (stores run lengths) ----------------
struct MinHeap {
    int* a; int sz; int cap;
    MinHeap(int capacity) : sz(0), cap(capacity) { a = new int[cap]; }
    ~MinHeap() { delete[] a; }

    void push(int x) {
        if (sz >= cap) return;
        a[sz] = x;
        int i = sz++;
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[p] <= a[i]) break;
            int t = a[p]; a[p] = a[i]; a[i] = t;
            i = p;
        }
    }

    int popMin() {
        if (sz == 0) return -1;
        int root = a[0];
        a[0] = a[--sz];
        int i = 0;
        while (true) {
            int l = 2*i + 1, r = 2*i + 2, s = i;
            if (l < sz && a[l] < a[s]) s = l;
            if (r < sz && a[r] < a[s]) s = r;
            if (s == i) break;
            int t = a[i]; a[i] = a[s]; a[s] = t;
            i = s;
        }
        return root;
    }

    int peekMin() { return (sz > 0) ? a[0] : -1; } // non-destructive min
};

// ---------------- BST (stores unique byte values) ----------------
struct TNode {
    unsigned char v;
    TNode* l;
    TNode* r;
    TNode(unsigned char vv) : v(vv), l(NULL), r(NULL) {}
};

static void bstInsert(TNode*& root, unsigned char v) {
    if (!root) { root = new TNode(v); return; }
    if (v < root->v) bstInsert(root->l, v);
    else if (v > root->v) bstInsert(root->r, v);
}

static int bstCount(TNode* root) {
    if (!root) return 0;
    return 1 + bstCount(root->l) + bstCount(root->r);
}

static void bstFree(TNode* root) {
    if (!root) return;
    bstFree(root->l);
    bstFree(root->r);
    delete root;
}

// ---------------- Compress ----------------
static bool compressFile(const string& inPath, const string& outPath) {
    ifstream in(inPath, ios::binary);
    if (!in) return false;

    ofstream out(outPath, ios::binary);
    if (!out) return false;

    out.write("RLE1", 4);

    RunQueue q;
    MinHeap heap(20000);
    TNode* bst = NULL;

    int first = in.get();
    if (first == EOF) { bstFree(bst); return true; } 
    unsigned char cur = (unsigned char)first;
    unsigned char cnt = 1;
    bstInsert(bst, cur);

    while (true) {
        int x = in.get();
        if (x == EOF) break;

        unsigned char b = (unsigned char)x;
        bstInsert(bst, b);

        if (b == cur && cnt < 255) cnt++;
        else {
            q.push(cnt, cur);
            heap.push((int)cnt);
            cur = b;
            cnt = 1;
        }
    }

    q.push(cnt, cur); // last run
    heap.push((int)cnt);

    unsigned char c, v;
    while (q.pop(c, v)) {
        out.put(c);
        out.put(v);
    }

    cout << "Stats: unique_symbols=" << bstCount(bst) 
         << ", min_run=" << heap.peekMin() << "\n";
    bstFree(bst);
    return true;
}

// ---------------- Decompress ----------------
static bool decompressFile(const string& inPath, const string& outPath) {
    ifstream in(inPath, ios::binary);
    if (!in) return false;

    char magic[4];
    if (!in.read(magic, 4)) return false;
    if (!(magic[0]=='R' && magic[1]=='L' && magic[2]=='E' && magic[3]=='1')) return false;

    ofstream out(outPath, ios::binary);
    if (!out) return false;

    MinHeap heap(20000);
    TNode* bst = NULL;

    while (true) {
        int c = in.get();
        int v = in.get();
        if (c == EOF || v == EOF) break;

        unsigned char count = (unsigned char)c;
        unsigned char value = (unsigned char)v;

        heap.push((int)count);
        bstInsert(bst, value);

        for (int i = 0; i < (int)count; i++) out.put(value);
    }

    cout << "Stats: unique_symbols=" << bstCount(bst) 
         << ", min_run=" << heap.peekMin() << "\n";
    bstFree(bst);
    return true;
}

// ---------------- Main ----------------
int main() {
    while (true) {
        cout << "\n1) Compress\n2) Decompress\n3) Exit\nChoose: ";
        int choice;
        cin >> choice;

        if (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (choice == 3) break;

        string inFile, outFile;
        cout << "Input file: ";
        cin >> inFile;
        cout << "Output file: ";
        cin >> outFile;

        if (choice == 1) cout << (compressFile(inFile, outFile) ? "OK\n" : "FAIL\n");
        else if (choice == 2) cout << (decompressFile(inFile, outFile) ? "OK\n" : "FAIL\n");
        else cout << "Invalid\n";
    }
    return 0;
}
