//
// Created by light on 19-12-23.
//

#ifndef LAB_KN_H
#define LAB_KN_H

#include "stdio.h"
#include "string.h"
#include<fstream>
#include<iostream>
#include <map>
#include <vector>
#include <set>
#include <queue>

using namespace std;
#define MAXN 310
#define inf 1000000000
#define _clr(x) memset(x,0xff,sizeof(int)*n)

/**
 * from https://github.com/lccycc/ACM_Template/blob/master/STL/ACM%E5%B8%B8%E7%94%A8%E6%A8%A1%E6%9D%BF_%E4%BA%8C%E5%88%86%E5%9B%BE%E6%9C%80%E4%BD%B3%E5%8C%B9%E9%85%8D(kuhn_munkras.txt
 * 二分图最佳匹配,kuhn munkras算法,邻接阵形式,复杂度O(m*m*n)
 * 返回最佳匹配值,传入二分图大小m,n和邻接阵mat,表示权值
 * match1,match2返回一个最佳匹配,未匹配顶点match值为-1
 * 一定注意m<=n,否则循环无法终止
 * 最小权匹配可将权值取相反数
 * match result
 * @param m
 * @param n
 * @param mat
 * @param match1
 * @param match2
 * @return
 */
int kuhn_munkras(int m, int n, int **mat, int *match1, int *match2) {
    int s[MAXN], t[MAXN], l1[MAXN], l2[MAXN], p, q, ret = 0, i, j, k;
    for (i = 0; i < m; i++)
        for (l1[i] = -inf, j = 0; j < n; j++)
            l1[i] = mat[i][j] > l1[i] ? mat[i][j] : l1[i];
    for (i = 0; i < n; l2[i++] = 0);
    for (_clr(match1), _clr(match2), i = 0; i < m; i++) {
        for (_clr(t), s[p = q = 0] = i; p <= q && match1[i] < 0; p++)
            for (k = s[p], j = 0; j < n && match1[i] < 0; j++)
                if (l1[k] + l2[j] == mat[k][j] && t[j] < 0) {
                    s[++q] = match2[j], t[j] = k;
                    if (s[q] < 0)
                        for (p = j; p >= 0; j = p)
                            match2[j] = k = t[j], p = match1[k], match1[k] = j;
                }
        if (match1[i] < 0) {
            for (i--, p = inf, k = 0; k <= q; k++)
                for (j = 0; j < n; j++)
                    if (t[j] < 0 && l1[s[k]] + l2[j] - mat[s[k]][j] < p)
                        p = l1[s[k]] + l2[j] - mat[s[k]][j];
            for (j = 0; j < n; l2[j] += t[j] < 0 ? 0 : p, j++);
            for (k = 0; k <= q; l1[s[k++]] -= p);
        }
    }
    for (i = 0; i < m; i++)
        ret += mat[i][match1[i]];
    return ret;
}

/**
 * matrix rows
 * @param fileName
 * @return
 */
int getRows(string &fileName) {
    ifstream fileStream;
    string tmp;
    int count = 0;
    fileStream.open(fileName, ios::in);
    if (fileStream.fail()) {
        cout << "fail!" << endl;
        return 0;
    } else {
        cout << "success!" << endl;
        while (getline(fileStream, tmp, '\n')) {
            if (tmp.size() > 0)
                count++;
        }
        fileStream.close();
        return count;
    }
}

/**
 * matrix cols
 * @param fileName
 * @return
 */
int getFileColumns(string &fileName) {
    ifstream fileStream;
    fileStream.open(fileName, ios::in);
    if (fileStream.fail()) {
        cout << "fail!" << endl;
        return 0;
    } else {
        cout << "success!" << endl;
        double tmp = 0;
        int count = 0;
        char c;
        c = fileStream.peek();
        while (('\n' != c) && (!fileStream.eof())) {
            fileStream >> tmp;
            ++count;
            c = fileStream.peek();
        }

        fileStream.close();
        return count;
    }
}

/**
 * read data
 * @param m
 * @param n
 * @param fileName
 * @param mat
 */
void readData(int m, int n, string &fileName, int **mat) {
    FILE *f;
    f = fopen(fileName.c_str(), "r");
    // read data
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(f, "%d", &mat[i][j]);
        }
    }

    fclose(f);
}


void buildExpandMatrix(int **expandMat, int **rawMat, int line) {
    for (int i = 0; i < 3 * line; i++)
        for (int j = 0; j < 3 * line; j++)
            if (i >= 0 && i < line && j >= 0 && j < line)
                expandMat[i][j] = rawMat[i][j];
            else if (i >= line && i < 2 * line && j >= line && j < 2 * line)
                expandMat[i][j] = rawMat[i - line][j - line];
            else if (i >= 2 * line && i < 3 * line && j >= 2 * line && j < 3 * line)
                expandMat[i][j] = rawMat[i - 2 * line][j - 2 * line];
}

void printMatrix(int **mat, int m, int n) {
    cout << "===========print matrix===========" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }
}

void freeMatrix(int **mat, int m, int *match1 = NULL, int *match2 = NULL) {
    for (int i = 0; i < m; ++i)
        delete[]mat[i];
    delete[]mat;
    if (match1) {
        delete match1;
        delete match2;
    }
}

void printMap(const map<int, int> &m) {
    for (auto each:m)
        cout << each.first << "     :   " << each.second << endl;
}

// build {color:vendor}
void buildCV(map<int, int> &cvMap, int *match1, int m) {
    // D is color and S is vendor
    for (int i = 0; i < m; ++i) {
        printf("D%d->S%d\n", i + 1, match1[i] + 1);
        cvMap.insert(make_pair(i + 1, match1[i] + 1));
    }
}

// build {vendor:color}
void buildVC(map<int, int> &vcMap, int *match1, int m) {
    // D is color and S is vendor
    for (int i = 0; i < m; ++i) {
        printf("D%d->S%d\n", i + 1, match1[i] + 1);
        vcMap.insert(make_pair(match1[i] + 1, i + 1));
    }
}

// build {types:area}
void buildTA(int **tMat, int m, int n, map<int, int> &typeArea) {
    // area data file name
    string areaFileName = "/home/light/work/崔老师实验/tu/data/area.txt";
    readData(m, n, areaFileName, tMat);
    // output Matrix
    printMatrix(tMat, m, n);

    for (int i = 0; i < m; i++)
        typeArea.insert(make_pair(tMat[i][0], tMat[i][1]));
}

// build {color:types}
void buildCT(int ctRow, int ctCol, int **ctMat, map<int, vector<int>> &ctMap, string &color_types) {
    for (int i = 0; i < ctRow; i++)
        for (int j = 0; j < ctCol; j++)
            ctMat[i][j] = -1;
    readData(ctRow, ctCol, color_types, ctMat);
    printMatrix(ctMat, ctRow, ctCol);

    for (int j = 0; j < ctCol; j++) {
        vector<int> v;
        for (int i = 1; i < ctRow; i++) {
            if (ctMat[i][j] != -1)
                v.push_back(ctMat[i][j]);
        }
        ctMap.insert(make_pair(j + 1, v));
    }
}

// use {color:types}  and {vendor:color} build {vendor:types}
void buildVT(map<int, vector<int>> &vtMap, map<int, vector<int>> &ctMap, map<int, int> &vcMap) {
    for (auto vc:vcMap) {
        for (auto elem:ctMap[vc.second])
            vtMap[vc.first].push_back(elem);
    }
}

template<typename T>
void printVxMap(const map<int, vector<T>> &m) {
    for (auto each:m) {
        cout << each.first << ":[ ";
        for (auto i:each.second) {
            cout << i << " ";
        }
        cout << "]" << endl;
    }
}

struct Node {
    int tag{0};        // node number
    int type{-1};       // node type
    int cycle{-1};      // node cycle
    int color{-1};      // node color -1 is invalid color
    int vendor{-1};       // node vendor
    bool isVisited{false}; // node is visited
    bool isDummyHead{false}; // node is or isn't dummyHead
    vector<Node *> next{};

    Node(bool isDHead = false) : isDummyHead(isDHead) {}

    friend ostream &operator<<(ostream &os, const Node *node) {
        os << "{tag:" << node->tag << ",type:" << node->type << ",cycle:" << node->cycle <<
           ",isVisited:" << node->isVisited << ",isDummyHead:" << node->isDummyHead << ",color:" <<
           node->color << ",vendor:" << node->vendor << "}";
        return os;
    }
};

void printAllMap(const map<int, vector<Node *>> &vNodeMap, const map<int, vector<int>> &vtMap,
                 const map<int, vector<int>> &vnMap, const map<int, vector<int>> &ctMap, const map<int, int> &vcMap) {
    cout << "vendor:color" << endl;
    printMap(vcMap);
    cout << "color:types" << endl;
    printVxMap(ctMap);
    cout << "vendor:types" << endl;
    printVxMap(vtMap);
    cout << "vendor:number" << endl;
    printVxMap(vnMap);
    cout << "vendor:nodes" << endl;
    printVxMap(vNodeMap);
}


/**
 * load raw data from local file
 * @param fileName
 * @param line
 * @param isOnce
 * @param mat
 */
void readRawMatrix(string fileName, int *line, bool *isOnce, int **mat = NULL) {
    char buf[1024];                //临时保存读取出来的文件内容
    string message;
    ifstream infile;
    infile.open(fileName);
    int lines = 0;
    if (infile.is_open())          //文件打开成功,说明曾经写入过东西
    {
        // good表示文件流是否正常 eof表示文件流是否到结束了
        while (infile.good() && !infile.eof()) {
            lines++;
            memset(buf, 0, 1024);
            infile.getline(buf, 1204);
            message = buf;
            if (*isOnce && lines == 4) {
                *isOnce = false;
                *line = stoi(message);
                break;
            }
            if (lines == 5) {
                int tmp;
                // read data
                for (int i = 0; i < *line; i++) {
                    for (int j = 0; j < *line; j++) {
                        infile >> mat[i][j];
                        cout << mat[i][j] << " ";
                    }
                    cout << endl;
                }
            }
        }
        infile.close();
    }
}


void buildVendorNode(map<int, vector<Node *>> &m_, const map<int, vector<int>> &vtMap,
                     const map<int, vector<int>> &vnMap) {
    for (auto vm:vtMap) {
        // add node types and node number
        for (int i = 0; i < vm.second.size(); i++) {
            Node *cur = new Node();
            cur->tag = vnMap.at(vm.first).at(i);
            cur->type = vm.second.at(i);
            m_[vm.first].push_back(cur);
        }
    }
}

#endif //LAB_KN_H
