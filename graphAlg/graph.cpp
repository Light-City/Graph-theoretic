#include "graph.h"


int main(int argc, char *argv[]) {

    string file = argv[1];

    string n_ip = argv[2];
    cout << "========================get a match {color:vendor}===============" << endl;
    int m, n;
    string fileName = "../data/graphColor/" + file + "_" + n_ip + "_weight.txt";
    cout << "file name is：" << fileName << endl;
    m = getRows(fileName);
    n = getFileColumns(fileName);

    int **mat = new int *[m];
    int *match1 = new int[m];
    int *match2 = new int[n];
    for (int i = 0; i < m; ++i) mat[i] = new int[n];

    readData(m, n, fileName, mat);  // fill data
    printMatrix(mat, m, n);     // print matrix

    int ret = kuhn_munkras(m, n, mat, match1, match2);  // get match result
    printf("minWeightSum = %d\n", -ret);
    printf("match: \n");


    // build {color:vendor}
    map<int, int> cvMap;
    buildCV(cvMap, match1, m);

    cout << "======================== build a map of {color:vendor} ===============" << endl;
    cout << "color:   vendor" << endl;
    printMap(cvMap);

    cout << "======================== build a map of {vendor:color} ===============" << endl;

    // build {vendor:color}
    map<int, int> vcMap;
    buildVC(vcMap, match1, m);
    cout << "vendor:   color" << endl;
    printMap(vcMap);

    cout << "======================== build a map of {type:area} ===============" << endl;

    int types_m = 4, types_n = 2;
    int **tMat = new int *[types_m]; // read data into types-area matrix    called tMat Matrix
    for (int i = 0; i < types_m; ++i) tMat[i] = new int[types_n];
    // build {types:area}
    map<int, int> typeArea;
    buildTA(tMat, types_m, types_n, typeArea);
    cout << "types :   area" << endl;
    printMap(typeArea);


    cout << "======================== build a map of {color:types} ===============" << endl;
    string color_types = "../data/graphColor/" + file + "_" + n_ip + "_cip.txt";
    int ctRow = getRows(color_types);
    int ctCol = getFileColumns(color_types);
    int **ctMat = new int *[ctRow];
    for (int i = 0; i < ctRow; ++i) ctMat[i] = new int[ctCol];
    map<int, vector<int>> ctMap;
    buildCT(ctRow, ctCol, ctMat, ctMap, color_types);

    printVxMap(ctMap);

    cout << "======================== build a map of {vendor:types} ===============" << endl;
    // use {color:types}  and {vendor:color} build {vendor:types}
    map<int, vector<int>> vtMap;
    buildVT(vtMap, ctMap, vcMap);
    printVxMap(vtMap);

    cout << "======================== build a map of {color:number} ===============" << endl;
    string color_number = "../data/graphColor/" + file + "_" + n_ip + "_cipr.txt";
    int **cnMat = new int *[ctRow];
    for (int i = 0; i < ctRow; ++i) cnMat[i] = new int[ctCol];
    map<int, vector<int>> cnMap;
    buildCT(ctRow, ctCol, cnMat, cnMap, color_number);

    printVxMap(cnMap);

    cout << "======================== build a map of {vendor:number} ===============" << endl;
    // use {color:number}  and {vendor:color} build {vendor:number}
    map<int, vector<int>> vnMap;
    buildVT(vnMap, cnMap, vcMap);
    printVxMap(vnMap);


    cout << "======================== build a map of {vendor:node} ===============" << endl;
    map<int, vector<Node *>> vNodeMap;
    buildVendorNode(vNodeMap, vtMap, vnMap);
    printVxMap(vNodeMap);


    cout << "======================== construct raw matrix ===============" << endl;

    string rawData = "../data/rawData/" + file + ".txt";
    int line = 0;
    bool isOnce = true;
    readRawMatrix(rawData, &line, &isOnce);     // get line

    int **rawMat = new int *[line];
    for (int i = 0; i < line; ++i) rawMat[i] = new int[line];

    readRawMatrix(rawData, &line, &isOnce, rawMat); // get matrix
    printMatrix(rawMat, line, line);

    // get the node tag of the recovery stage according to the line
    set<int> s;
    [line](set<int> &s) {
        for (int i = 2 * line; i <= 3 * line; i++)
            s.insert(i);
    }(s);

    cout << "======================== construct expand matrix ===============" << endl;
    int **expandMat = new int *[3 * line];
    for (int i = 0; i < 3 * line; ++i) {
        expandMat[i] = new int[3 * line];
        memset(expandMat[i], 0, sizeof(int) * (3 * line));
    }

    buildExpandMatrix(expandMat, rawMat, line);

    printMatrix(expandMat, 3 * line, 3 * line);


    cout << "======================== construct a graph ======================" << endl;
    GraphIp graphIp(vNodeMap, 3 * line, 3 * line);
    vector<Node *> dummyHeadVec = graphIp.buildGraph(expandMat, vcMap, vNodeMap);
    graphIp.setDummyHead(dummyHeadVec);

    graphIp.printAllGraph(dummyHeadVec, true);       // true for dfs print or false for bfs print
    graphIp.printAllNode();

    cout << "======================== print all map ===============" << endl;
    printAllMap(vNodeMap, vtMap, vnMap, ctMap, vcMap);


    cout << "======================== get Raw Area and Cycle ===============" << endl;

    int rawCycle = graphIp.getAllStageCycle();
    cout << "raw time cycle is " << rawCycle << endl;
    int rawArea = graphIp.getRawArea(typeArea, vNodeMap);
    cout << "raw area is " << rawArea << endl;

    cout << "======================== detect a graph ======================" << endl;
    // from color 1 to detect a graph
    vector<vector<Node *>> res = graphIp.detectAllVendor(vNodeMap, cvMap);

    cout << "======================== get result ======================" << endl;
    cout << "color:   vendor" << endl;
    printMap(cvMap);
    int minSum = -ret;
    cout << "minWeightSum is " << minSum << endl;
    int minCycle = graphIp.getAllStageCycle();


    cout << "raw time cycle is " << rawCycle << endl;
    cout << "raw area is " << rawArea << endl;

    cout << "minimum time cycle is " << minCycle << endl;
    int minArea = graphIp.minArea(typeArea, res);
    cout << "minimum area is " << minArea << endl;

    graphIp.setGraphResult(rawCycle, rawArea, minSum, minCycle, minArea);

    string saveFileName = "../output/" + file + "_" + n_ip + "_res.txt";
    graphIp.saveResult(saveFileName, mat, m, n, cvMap);


    cout << "======================== free malloc memory ======================" << endl;
    freeMatrix(mat, m, match1, match2);
    freeMatrix(tMat, types_m);
    freeMatrix(ctMat, ctRow);
    freeMatrix(cnMat, ctRow);
    freeMatrix(rawMat, line);
    freeMatrix(expandMat, 3 * line);
    cout << "free successful !" << endl;

    return 0;
}
