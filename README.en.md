# Graph-theoretic

## Project description

## 2. Description of content

### 2.1 Directory description

The complete contents of the directory are as follows:

```
├── data
│   ├── graphColor
│   │   ├── 0_0_cipr.txt
│   │   ├── 0_0_cip.txt
│   │   ├── 0_0_weight.txt
│   │   ├── ...
│   └── rawData
│       ├── 0.txt
│       ├── ...
│       ├── area.txt
│       └── weight.csv
├── graphAlg
│   ├── a.out
│   ├── graphColor.ipynb
│   ├── graph.h
│   ├── graph.cpp
│   └── util.h
├── output
│   ├── 1_0_res.txt
│   ├── ...
└── README.md
```

The above directories can be divided into:

- Dataset directory

  - `rawData`

  For the original data set catalog, `weight.csv`different types of IP core data sets are produced for different manufacturers, and `area.txt`the areas corresponding to different types of nodes.

  - `graphColor`

  For the `graphColor.ipynb`output path

- Code directory

  - `graphColor.ipynb`

    Is a [jupyter-notebook](https://jupyter.org/) file, the main functions are as follows:

    - Data preprocessing and graph coloring
    - Relationship between color and node type
    - The relationship between colors and nodes
    - Weight matrix matching minimum weight in bipartite graph

  - `graph.cpp`

    It is the C ++ main program. Its specific functions are implemented in `graph.h`and `util.h`. The main functions are as follows:

    - Read `data/graphColor`the data set in the directory
    - Output the following experimental results to the output directory
    - Optimize match to complete the optimal match based on bipartite graph
    - Minimize cycle and complete time-based IP core scheduling
    - Optimize area and complete area-based IP core scheduling

- Results directory

  - `output`Output results for experiment

### 2.2 Data description

- rawData

  Description of the original data set, which contains some benchmark data sets.

- graphColor

  `graphColor.ipynb`Loading each raw dataset will output 3 files:

  `1_0_cip.txt`, `0_0_cipr.txt`,`0_0_weight.txt`

  1 represents the original dataset number, 0 represents the number of trusted IP cores, cip represents the color and node type dataset, cipr represents the color and node dataset, and weight represents the output weight dataset.

  Among them, cip and cipr are understood by columns, which are the nodes or node types corresponding to each color.

- output

  `1_0_res.txt`Represents the experimental results of the first data set with 0 trusted IP cores

## 3. Instructions for use

### 3.1 Environmental requirements

- Python 3.x
  - jupyter-notebook
  - numpy
  - networkx
  - matplotlib
  - pandas
  - copy
- C ++
  - Compiler supporting at least C ++ 11

Our development environment is:

- Ubuntu18.04 + Anaconda3 + GCC8.3

### 3.2 How to use

- Please make sure the 3.1 environment is correct

- Please run the `graphColor.ipynb`file and the experiment results will be stored under the graphColor folder.

- Please run`graph.cpp`

  - It works as follows:

  ```
  g ++ graph.cpp -std = c ++ 11 
  ./a.out
  ```

- View experimental results (enter the output directory)