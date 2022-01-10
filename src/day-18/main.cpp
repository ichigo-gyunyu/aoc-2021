#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

struct Node {
    bool isLeaf = false;
    Node *left = NULL;
    Node *right = NULL;
    Node *parent = NULL;
    int val = -1;

    Node(int x, Node *p = NULL) : val(x) {
        isLeaf = true;
        parent = p;
    }
    Node(Node *p = NULL) { parent = p; }
};

// free up memory
void cleanup(Node *n) {
    if (!n)
        return;
    cleanup(n->left);
    cleanup(n->right);
    delete n;
}

Node *parse_str(std::string s) {
    assert(s[0] == '[');
    assert(s[s.size() - 1] == ']');
    Node *root = new Node();
    Node *p = root;

    size_t i = 1;
    while (i < s.size() - 1) {
        bool insert_right = false;
        if (s[i] == ',') {
            insert_right = true;
            ++i;
            assert(i < s.size() - 1);
        }

        if (s[i] == '[') {
            Node *tmp = new Node(p);
            if (insert_right)
                p->right = tmp;
            else
                p->left = tmp;
            p = tmp;
        } else if (s[i] == ']') {
            assert(p->parent != NULL);
            p = p->parent;
        } else {
            int x = s[i] - '0';
            Node *tmp = new Node(x, p);
            if (insert_right)
                p->right = tmp;
            else
                p->left = tmp;
        }

        ++i;
    }

    return root;
}

int64_t part1(const std::vector<Node *> &nums) {
    Node *res = nums[0];

    auto snailfish_add = [&](Node *n) {
        Node *new_root = new Node();
        assert(!res->parent);
        assert(!n->parent);
        res->parent = new_root;
        n->parent = new_root;
        new_root->left = res;
        new_root->right = n;
        res = new_root;
    };

    std::function<Node *(Node *, int)> can_explode = [&](Node *n,
                                                         int depth) -> Node * {
        if (!n)
            return NULL;

        if (depth == 4 && !n->isLeaf)
            return n;

        auto l = can_explode(n->left, depth + 1);
        if (l)
            return l;
        auto r = can_explode(n->right, depth + 1);
        if (r)
            return r;
        return NULL;
    };

    std::function<Node *(Node *)> can_split = [&](Node *n) -> Node * {
        if (!n)
            return NULL;

        if (n->val > 9)
            return n;

        auto l = can_split(n->left);
        if (l)
            return l;
        auto r = can_split(n->right);
        if (r)
            return r;
        return NULL;
    };

    auto explode = [](Node *n) {
        assert(n->left->isLeaf && n->right->isLeaf);
        int l = n->left->val;
        int r = n->right->val;

        Node *p = n;
        while (p->parent && p->parent->left == p) {
            p = p->parent;
        }
        p = p->parent;
        if (p) {
            p = p->left;
            while (!p->isLeaf)
                p = p->right;
            p->val += l;
        }

        p = n;
        while (p->parent && p->parent->right == p) {
            p = p->parent;
        }
        p = p->parent;
        if (p) {
            p = p->right;
            while (!p->isLeaf)
                p = p->left;
            p->val += r;
        }

        Node *t = new Node(0, n->parent);
        if (n->parent->left == n)
            n->parent->left = t;
        else
            n->parent->right = t;

        delete n->left;
        delete n->right;
        delete n;
    };

    auto split = [](Node *n) {
        if (!n)
            return;
        int l = n->val / 2;
        int r = (n->val + 1) / 2;
        Node *t = new Node(n->parent);
        Node *tl = new Node(l, t);
        Node *tr = new Node(r, t);
        t->left = tl;
        t->right = tr;

        if (n->parent->left == n)
            n->parent->left = t;
        else
            n->parent->right = t;

        delete n;
    };

    for (size_t i = 1; i < nums.size(); i++) {
        snailfish_add(nums[i]);

        while (true) {
            auto ex = can_explode(res, 0);
            auto sp = can_split(res);
            if (!ex && !sp)
                break;
            if (ex) {
                explode(ex);
                continue;
            }
            split(sp);
        }
    }

    std::function<int(Node *)> snailfish_mag = [&](Node *n) {
        if (n->isLeaf)
            return n->val;
        return 3 * snailfish_mag(n->left) + 2 * snailfish_mag(n->right);
    };

    int ans = snailfish_mag(res);

    cleanup(res); // free

    return ans;
}

int64_t part2(std::vector<std::string> ins) {

    auto snailfish_add = [&](Node *n1, Node *n2) {
        Node *new_root = new Node();
        n1->parent = new_root;
        n2->parent = new_root;
        new_root->left = n1;
        new_root->right = n2;
        return new_root;
    };

    std::function<Node *(Node *, int)> can_explode = [&](Node *n,
                                                         int depth) -> Node * {
        if (!n)
            return NULL;

        if (depth == 4 && !n->isLeaf)
            return n;

        auto l = can_explode(n->left, depth + 1);
        if (l)
            return l;
        auto r = can_explode(n->right, depth + 1);
        if (r)
            return r;
        return NULL;
    };

    std::function<Node *(Node *)> can_split = [&](Node *n) -> Node * {
        if (!n)
            return NULL;

        if (n->val > 9)
            return n;

        auto l = can_split(n->left);
        if (l)
            return l;
        auto r = can_split(n->right);
        if (r)
            return r;
        return NULL;
    };

    auto explode = [](Node *n) {
        assert(n->left->isLeaf && n->right->isLeaf);
        int l = n->left->val;
        int r = n->right->val;

        Node *p = n;
        while (p->parent && p->parent->left == p) {
            p = p->parent;
        }
        p = p->parent;
        if (p) {
            p = p->left;
            while (!p->isLeaf)
                p = p->right;
            p->val += l;
        }

        p = n;
        while (p->parent && p->parent->right == p) {
            p = p->parent;
        }
        p = p->parent;
        if (p) {
            p = p->right;
            while (!p->isLeaf)
                p = p->left;
            p->val += r;
        }

        Node *t = new Node(0, n->parent);
        if (n->parent->left == n)
            n->parent->left = t;
        else
            n->parent->right = t;

        delete n->left;
        delete n->right;
        delete n;
    };

    auto split = [](Node *n) {
        if (!n)
            return;
        int l = n->val / 2;
        int r = (n->val + 1) / 2;
        Node *t = new Node(n->parent);
        Node *tl = new Node(l, t);
        Node *tr = new Node(r, t);
        t->left = tl;
        t->right = tr;

        if (n->parent->left == n)
            n->parent->left = t;
        else
            n->parent->right = t;

        delete n;
    };

    std::function<int(Node *)> snailfish_mag = [&](Node *n) {
        if (n->isLeaf)
            return n->val;
        return 3 * snailfish_mag(n->left) + 2 * snailfish_mag(n->right);
    };

    auto reduce = [&](Node *n) {
        while (true) {
            auto ex = can_explode(n, 0);
            auto sp = can_split(n);
            if (!ex && !sp)
                break;
            if (ex) {
                explode(ex);
                continue;
            }
            split(sp);
        }
    };

    int ans = -1;
    for (size_t i = 0; i < ins.size(); i++) {
        for (size_t j = 0; j < ins.size(); j++) {
            if (i == j)
                continue;

            auto n1 = parse_str(ins[i]);
            auto n2 = parse_str(ins[j]);
            auto n = snailfish_add(n1, n2);
            reduce(n);
            ans = std::max(ans, snailfish_mag(n));
            cleanup(n);

            n1 = parse_str(ins[i]);
            n2 = parse_str(ins[j]);
            n = snailfish_add(n2, n1);
            reduce(n);
            ans = std::max(ans, snailfish_mag(n));
            cleanup(n);
        }
    }

    return ans;
}

int main() {

    std::string in;
    std::vector<Node *> nums;
    std::vector<std::string> ins;
    while (std::cin >> in) {
        ins.push_back(in);
    }

    for (auto e : ins) {
        nums.push_back(parse_str(e));
    }

    auto start = std::chrono::high_resolution_clock::now();
    int64_t part1_ans = part1(nums);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    int64_t part2_ans = part2(ins);
    end = std::chrono::high_resolution_clock::now();
    auto time2 =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nPart 1: " << part1_ans << "\nTime: " << time1 << "ms"
              << std::endl;
    std::cout << "\nPart 2: " << part2_ans << "\nTime: " << time2 << "ms"
              << std::endl;
}
