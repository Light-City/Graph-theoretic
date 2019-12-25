//
// Created by light on 19-12-24.
//

#ifndef LAB_GRAPH_H
#define LAB_GRAPH_H

#include "km.h"

class GraphIp {
private:
    int row_;
    int col_;
    map<int, vector<Node *>> m_;
    int preStageMaxCycle{-1};       // normal and recalculate stage max cycle
    int allStageMaxCycle{-1};
    Node *dummyHead1_{nullptr};
    Node *dummyHead2_{nullptr};
    Node *dummyHead3_{nullptr};
    struct GraphResult {
        int minSum;
        int minCycle;
        int minArea;
    };
    GraphResult graphResult;
public:
    GraphIp(map<int, vector<Node *>> m, int row, int col) : row_(row), col_(col) {
        for (auto each:m)
            for (auto elem:each.second)
                m_[each.first].push_back(elem);
    }

    ~GraphIp() {

    }

    // time space is O(n*log(n))
    Node *getNode(int target) {
        for (auto each:m_)
            for (auto elem:each.second)
                if (elem->tag == target)
                    return elem;
    }

    // get all actual node
    map<int, Node *> getAllNode() {
        // only once to find all node insert a set
        map<int, Node *> allNode;
        for (int i = 1; i < row_ + 1; i++) {
            Node *node = getNode(i);
            allNode.insert(make_pair(i, node));
        }
        return allNode;
    }

    // no dummyHead
    void printAllNode() {
        cout << "------------Graph ALL Node Info----------" << endl;
        cout << "nodeTag:nodeInfo" << endl;
        cout << "-----------------------------------------" << endl;
        map<int, Node *> allNode = getAllNode();
        int i = 0;
        int raw = row_ / 3;
        for (auto each:allNode) {
            if (i == 2 * raw)
                cout << "--------------------恢复阶段-----------------" << endl;
            else if (i == 0)
                cout << "--------------------检测阶段-----------------" << endl;
            i++;
            cout << each.first << " " << each.second << endl;
        }
    }

    // for print node visited clear
    void isVisitedClear() {
        map<int, Node *> allNode = getAllNode();
        for (auto each:allNode)
            each.second->isVisited = false;
    }


    vector<Node *> buildGraph(int **mat, map<int, int> &vcMap, map<int, vector<Node *>> vNodeMap) {
        set<int> head;
        map<int, Node *> allNode = getAllNode();
        // col data is or isn't 0
        for (int j = 0; j < col_; j++) {
            bool flag = true;
            for (int i = 0; i < row_; i++)
                if (mat[i][j] != 0) {
                    flag = false;
                    break;
                }
            // all col data is 0
            if (flag == true) {
                head.insert(j + 1);
            }
        }
        int raw_Row = row_ / 3;
        // create dummyHead
        Node *dummyHead1 = new Node(true);
        Node *dummyHead2 = new Node(true);
        Node *dummyHead3 = new Node(true);

        for (int i = 0; i < row_; i++) {
            Node *node = allNode[i + 1];
            // is head node
            if (head.count(i + 1) > 0) {
                if (i < raw_Row)
                    dummyHead1->next.push_back(node);
                else if (i >= raw_Row && i < 2 * raw_Row)
                    dummyHead2->next.push_back(node);
                else if (i >= 2 * raw_Row && i < row_)
                    dummyHead3->next.push_back(node);

            }
            for (int j = 0; j < col_; j++) {
                if (mat[i][j] == 1) {
                    Node *cur = allNode[j + 1];
                    // get node
                    node->next.push_back(cur);
                }
            }
        }
        vector<Node *> graphDummyHeadVec{dummyHead1, dummyHead2, dummyHead3};
        // dfs for fill graph node cycle
        vector<vector<Node *>> allRes;
        vector<Node *> halfRes{new Node()};
        for (int i = 0; i < graphDummyHeadVec.size(); i++) {
            if (i <= 1)  // normal and recalculate
                dfsPrintGraphLevel(graphDummyHeadVec[i], allRes, halfRes, false, false);
            else {      // recover
                graphDummyHeadVec[i]->cycle = preStageMaxCycle - 1;
                halfRes.clear();
                halfRes.push_back(graphDummyHeadVec[i]);
                dfsPrintGraphLevel(graphDummyHeadVec[i], allRes, halfRes, false, true);
            }
        }

        // dfs for fill graph node color
        for (auto each:vNodeMap)
            for (auto node:each.second) {
                node->color = vcMap[each.first];
                node->vendor = each.first;
            }

        return graphDummyHeadVec;
    }

    int getRecoverStartCycle() const {
        return preStageMaxCycle;
    }

    void setDummyHead(vector<Node *> &dummyHeadVec) {
        int i = 1;
        for (auto each:dummyHeadVec) {
            switch (i) {
                case 1:
                    dummyHead1_ = each;
                case 2:
                    dummyHead2_ = each;
                case 3:
                    dummyHead3_ = each;
            }
        }
    }

    int getAllStageCycle() {
        vector<vector<Node *>> m1;
        vector<Node *> m2;
        //  print and isRecover not update preStageMaxCycle
        dfsPrintGraphLevel(dummyHead3_, m1, m2, true, true);
        return allStageMaxCycle;
    }

    // use dfs print
    // and
    // use dfs init cycle
    // from initTag start update cycle until end node(no child node)
    void dfsPrintGraphLevel(Node *head, vector<vector<Node *>> &m1, vector<Node *> m2,
                            bool isPrint = true, bool isRecover = false) {
        Node *cur = head;

        if (!isPrint) {
            try {
                cur->cycle = m2.back()->cycle + 1;
            } catch (...) {
                cout << __FUNCTION__ << ":" << __LINE__ << endl;
            }
        }

        m2.push_back(cur);

        // global graph cycle
        if (cur->tag == row_)
            allStageMaxCycle = cur->cycle;

        if (cur->next.empty()) {
            if (isRecover == false && cur->cycle > preStageMaxCycle)
                preStageMaxCycle = cur->cycle;
            m1.push_back(m2);
            return;
        }
        for (auto each:cur->next)
            dfsPrintGraphLevel(each, m1, m2, isPrint, isRecover);
    }

    // bfs print
    void bfsPrintGraph(Node *dummyHead) {
        cout << "Graph bfs print" << endl;
        queue<Node *> q;
        q.push(dummyHead);
        while (!q.empty()) {
            Node *cur = q.front();
            q.pop();
            if (!cur->isVisited) {
                cout << cur->tag << " ";
                cur->isVisited = true;
            }
            if (!cur->next.empty()) {
                for (auto each:cur->next) {
                    q.push(each);
                }
            }
        }
        cout << endl;

        // recover isVisited
        isVisitedClear();
    }

    void printAllGraph(vector<Node *> dummyHeadVec, bool isdfs = true) {
        int count = 0;
        for (auto subGraph:dummyHeadVec) {
            vector<vector<Node *>> allRes;
            vector<Node *> halfRes;
            count++;
            cout << "----------Graph" << count << "----------" << endl;
            if (isdfs) {
                dfsPrintGraphLevel(subGraph, allRes, halfRes, true, true);
                for (auto each:allRes) {
                    for (auto elem:each)
                        cout << elem << " ";
                    cout << endl;
                }
            } else {
                bfsPrintGraph(subGraph);
            }
        }
    }

    vector<vector<Node *>>
    detectAllVendor(map<int, vector<Node *>> &vNodeMap, map<int, int> &cvMap) {
        vector<vector<Node *>> res;
        // from color 1 to color other
        // cvMap:{color:vendor}
        // vNodeMap:{vendor:vector<Node*>}

        for (auto each:cvMap) {
            vector<Node *> vendorForAllNode = vNodeMap[each.second];

            // scan all vendorForAllNode set to get some match
            for (int i = 0; i < vendorForAllNode.size(); i++) {
                if (vendorForAllNode[i]->isVisited == false) {
                    vector<Node *> ipCore;
                    ipCore.push_back(vendorForAllNode[i]);
                    vendorForAllNode[i]->isVisited = true;
//                cout << vendorForAllNode[i] << endl;
                    // node isn't visited and i!=j
                    for (int j = 0; j < vendorForAllNode.size(); j++) {
                        if (vendorForAllNode[j]->isVisited == false) {
                            if (vendorForAllNode[i]->type == vendorForAllNode[j]->type) {
                                // Nodes can not be reused
                                if (vendorForAllNode[i]->cycle == vendorForAllNode[j]->cycle) {
                                    vector<vector<Node *>> allScan;
                                    vector<Node *> scanCur{vendorForAllNode[j]};
                                    if (vendorForAllNode[j]->tag <= 2 * (row_ / 3)) {
                                        dfsPrintGraphLevel(vendorForAllNode[j], allScan, scanCur, false,
                                                           false);   // cycle ++  update preStageMaxCycle

                                        // not recover update recover level
                                        vector<vector<Node *>> allRoute;
                                        vector<Node *> halfRes;
//                                        cout << "preStageMaxCycle=" << preStageMaxCycle << endl;
                                        dummyHead3_->cycle = preStageMaxCycle - 1;
                                        halfRes.push_back(dummyHead3_);
                                        dfsPrintGraphLevel(dummyHead3_, allRoute, halfRes, false);
                                    } else {
                                        dfsPrintGraphLevel(vendorForAllNode[j], allScan, scanCur, false,
                                                           true);   // not update preStageMaxCycle
                                    }
                                }
                                // Nodes can be reused
                                ipCore.push_back(vendorForAllNode[j]);
                                vendorForAllNode[j]->isVisited = true;
                            }
                        }
                    }
                    res.push_back(ipCore);

                    for (auto each:ipCore) {
                        cout << each << endl;
                    }
                    cout << "--------------------------" << endl;
                }
            }
        }
        return res;
    }

    int minArea(map<int, int> &typeArea, vector<vector<Node *>> &res) {
        int minArea = 0;
        // typeArea   type:area
        for (auto each:res) {
            for (auto node:each) {
                minArea += typeArea[node->type];
                break;
            }
        }

        return minArea;
    }

    void setGraphResult(int minSum, int minCycle, int minArea) {
        graphResult.minSum = minSum;
        graphResult.minCycle = minCycle;
        graphResult.minArea = minArea;
    }

    void saveResult(string fileName, int **weightMatrix, int m, int n, map<int, int> &cvMap) {
        ofstream outfile(fileName);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (j <= n - 2)
                    outfile << weightMatrix[i][j] << " ";
                else if (j == n - 1)
                    outfile << weightMatrix[i][j] << "\n";
            }
        }
        outfile << "color :" << "vendor" << "\n";
        for (auto each:cvMap)
            outfile << each.first << "     :   " << each.second << "\n";
        outfile << "minimum weight sum is " << graphResult.minSum << "\n";
        outfile << "minimum time cycle is " << graphResult.minCycle << "\n";
        outfile << "minimum area is       " << graphResult.minArea << "\n";

        outfile.close();
    }
};


#endif //LAB_GRAPH_H
