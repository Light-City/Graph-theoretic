# Graph-theoretic

[English README](./README.md)
## 1.项目描述

## 2.内容描述

### 2.1 目录描述

完整的目录内容如下：

```
├── data
│   ├── graphColor
│   │   ├── 0_0_cipr.txt
│   │   ├── 0_0_cip.txt
│   │   ├── 0_0_weight.txt
│   │   ├── ...
│   └── rawData
│       ├── 0.txt
│       ├── ...
│       ├── area.txt
│       └── weight.csv
├── graphAlg
│   ├── a.out
│   ├── graphColor.ipynb
│   ├── graph.h
│   ├── graph.cpp
│   └── util.h
├── output
│   ├── 1_0_res.txt
│   ├── ...
└── README.md
```

以上目录可以划分为：

- 数据集目录
  - `rawData`

  为原始数据集目录，`weight.csv`为不同厂商生产不同类型IP核数据集，`area.txt`为不同类型节点对应的面积。

  - `graphColor`

  为``graphColor.ipynb``输出结果路径

- 代码目录

  - `graphColor.ipynb`

    是一个[jupyter-notebook](https://jupyter.org/)文件，主要功能如下：
    - 数据预处理及图着色
    - 颜色与节点类型的关系
    - 颜色与节点的关系
    - 二分图最小权重匹配的权重矩阵

  - `graph.cpp`

    是C++主程序，其具体函数实现在`graph.h`、`util.h`，主要功能如下：

    - 读取`data/graphColor`目录下的数据集
    - 输出以下实验结果到output目录下

    - 最优化match，完成基于二分图的最优匹配
    - 最小化cycle，完成基于时间的IP核调度
    - 最优化area，完成基于面积的IP核调度

- 结果目录
  - `output`为实验输出结果目录

### 2.2 数据描述

- rawData

  原始数据集描述，里面是一些基准数据集。

- graphColor

  `graphColor.ipynb`加载每个原始数据集会输出3个文件：

  `1_0_cip.txt`、`0_0_cipr.txt`、`0_0_weight.txt`

  1 表示原始数据集编号、0表示受信任的IP核个数、cip代表颜色与节点类型数据集、cipr代表颜色与节点数据集、weight代表输出权重数据集。

  其中cip与cipr都是按列来理解，分别是每一种颜色对应的节点或节点类型。

- output

  `1_0_res.txt`表示第一个数据集在0个受信任IP核情况下的实验结果。

## 3.使用说明

### 3.1 环境要求

- Python 3.x 
  - jupyter-notebook
  - numpy
  - networkx
  - matplotlib
  - pandas
  - copy

- C++
  - 至少支持C++11的编译器

我们开发环境为：

- Ubuntu18.04+Anaconda3+GCC8.3

### 3.2 如何使用

- 请确保3.1环境正确

- 请运行`graphColor.ipynb`文件，将在graphColor文件夹下存储实验结果。

- 请运行`graph.cpp`
  - 运行方式如下：

  ```cpp
  g++ graph.cpp -std=c++11
  ./a.out
  ```

- 查看实验结果(进入output目录)