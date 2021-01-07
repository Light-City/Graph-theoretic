#!/usr/bin/env python
# coding: utf-8

# # Data preprocessing and graph theory implementation

# ## step 1 Load library

from numpy import *
import networkx as nx                   
import matplotlib.pyplot as plt 
import pandas as pd
import copy
import sys
import warnings
warnings.filterwarnings('ignore')

# ## step 2 Read Local File


input_list = sys.argv

if len(input_list) != 3:
    sys.exit(-1)

file_number = int(input_list[1])
def openFile(file_number):
    data = []
    with open("../data/rawData/"+ str(file_number)+".txt","r") as f:
        data=f.readlines()
    return data




data = openFile(file_number)



# get node number
'''
for example:
#nodes
3

return 3
'''
node_number = int(data[3])
node_number




# get types 
'''
for example:
#types
 0 0 0

return ['0','0','0']
'''
types = data[-1].strip().split(' ')
types



# According to the sequence of IP cores
def seqList(types):
    type_set = set()
    typeSetSeq = []
    for i in types:
        if i not in type_set:
            type_set.add(i)
            typeSetSeq.append(i)
    return typeSetSeq


typeSetSeq = seqList(types)
typeSetSeq



# Enter n trusted IP cores

n_ip = int(input_list[2])
if n_ip>len(typeSetSeq):
    print("trusted IP cores must <= type number")
    sys.exit(-1)
# ## step 3 Create Raw Matrix

# create matrix
'''
 0 1 1
 0 0 0
 0 0 0
'''

raw_matrix = zeros((node_number,node_number),dtype=int)    
# print(raw_matrix)

# Get all matrix rows
lines = data[5:-2]
# print(lines)




# get raw matrix
def getMatrix(lines):
    # format list for lines
    raw_matrix_row = 0                      # Represents the rows of the matrix, starting from row 0
    for line in lines:             #Read the data in lines line by line
        list = line.strip().strip('\n').split(' ')      #Process line-by-line data: strip means removing the '\ n' from the head and tail, split means splitting the line data with spaces, and then returning the processed line data to the list
        raw_matrix[raw_matrix_row:] = list[:node_number]                    #Place the processed data in square matrix A. list [0: 3] means that the 0,1,2 columns of the list are placed in the A_row row in matrix A
        raw_matrix_row+=1        #Then continue reading the next row of matrix A                        
        #print(line)



getMatrix(lines)
# print
# print(raw_matrix)


# ## step 4 Matrix transformation and expansion



# Convert to symmetric matrix
def toNoDirectGraph(matrix,n):
    for i in range(n):
        for j in range(n):
            if matrix[i][j]==1:
                matrix[j][i]=matrix[i][j]
    return matrix

# Matrix expansion
def generateMatrix(matrix,n):
    actual_number=n*3
    bigMatrix = zeros((actual_number,actual_number),dtype=int) 
    for i in range(actual_number):
        for j in range(actual_number):
            # Start filling the upper left corner
            if i<n and j<n:
                bigMatrix[i][j]=matrix[i][j]
                # Connecting special relationships
                bigMatrix[i][i+n]=1
                bigMatrix[i][i+2*n]=1
            if i==0 and j<n:    
                bigMatrix[j+n][j+2*n]=1
            # Start expanding the middle section
            if (i>=n and i<2*n) and (j>=n and j<2*n):
                bigMatrix[i][j]=matrix[i-n][j-n]
            # Start expanding the bottom right corner
            if (i>=2*n and i<3*n) and (j>=2*n and j<3*n):
                bigMatrix[i][j]=matrix[i-2*n][j-2*n]
    return bigMatrix




bigMatrix=generateMatrix(raw_matrix,node_number)
# print(bigMatrix)



bigMatrix = toNoDirectGraph(bigMatrix,3*node_number) 
# print(bigMatrix)
# print(bigMatrix.shape)


# ## step 5 Modify the matrix based on the trusted IP core


# time space is O(n)
def getLoc(types,seq):
    # Get the index of the trusted IP core from the type list (actual position -1)
    pos = []
    for i in range(len(types)):
        if types[i] in seq:
            pos.append(i)
    return pos



# Chinese: n如果等于typeSetSeq长度，表示全部ip核受信任,直接计算vendor1的价格,直接调到后面执行vendor计算
# English: If n is equal to the length of typeSetSeq, it means that all IP cores are trusted, and the price of vendor1 is directly calculated, which is directly transferred to the vendor calculation later.

# Chinese: 否则接着往下执行
# English: Otherwise, proceed to the next step

pos=[]

# Some are trusted, some are untrusted, remove trusted IP cores
if n_ip>0 and n_ip<len(typeSetSeq):
    frontSeq  = typeSetSeq[:n_ip]  # Trusted ip core
    # print(frontSeq)
    pos = getLoc(types,frontSeq)   # The index of the trusted IP core
    # print(pos)
    # Expand pos to triple length
    pos_1 = [i+node_number for i in pos]
    # print(pos_1)
    pos_2 = [i+2*node_number for i in pos]
    # print(pos_2)
    pos = pos+pos_1+pos_2
    # print(pos)   # The index corresponding to the IP core expanded three times
    # Modify bigMatrix
    for i in range(3*node_number):
        if i in pos:
            for j in range(3*node_number):
                bigMatrix[i][j]=0
        else:
            for j in range(3*node_number):
                if j in pos:
                    bigMatrix[i][j]=0
    bigMatrix



bigMatrix


# ## step 6 Create graphs and graph coloring


# takes matrix and creates a undirected graph
def CreateGraph():
    df = pd.DataFrame(bigMatrix)
    # create Graph
    G = nx.from_pandas_adjacency(df)
    return G


G = CreateGraph()



# Find the minimum color needed for coloring
# SLF method 
# https://www.semanticscholar.org/paper/Classical-Coloring-of-Graphs-Kosowski-Manuszewski/00d9eb1cb59935f6e0c406855c2d00d8a646dbff
colors = nx.coloring.greedy_color(G, strategy='saturation_largest_first')
# print(colors)

colors = [colors[k] for k in sorted(colors.keys())]
# print(colors)

#draws the graph and displays the weights on the edges
def DrawGraph(G,colors):
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels = True, node_color = colors, edge_color = 'black' ,width = 1, alpha = 0.7)  #with_labels=true is to show the node number in the output graph
    plt.plot()
    plt.savefig('../data/graphColor/'+str(file_number)+'_'+str(n_ip) + '.png', transparent=True)
    print("gcp png save ok")

DrawGraph(G,colors)


# Get the minimum number of graph coloring needed
colors_set = set()
for i in colors:
    colors_set.add(i)

minimum_color = len(colors_set)
print("at least "+str(minimum_color)+" colors")

# get a dict for {color:[nodes]}
result_dict = {}
for i in range(len(colors)):
    if colors[i] not in result_dict:
        result_dict[colors[i]]=[]
    result_dict[colors[i]].append(i+1)
# result_dict
# Note that the Index in the figure starts from 0, here is 1


# ## step 7 Store color and node relationships


# list lengths are not equal, in order to store correctly, fill with -1
save_dict = copy.deepcopy(result_dict)
max_len = 0
for k,v in save_dict.items():
    max_len=max(max_len,len(v))
for k,v in save_dict.items():
    if len(v)<max_len:
        for i in range(max_len-len(v)):
            v.append(-1)
# save_dict


import pandas as pd
save = pd.DataFrame(save_dict)

save.to_csv('../data/graphColor/'+str(file_number)+'_'+str(n_ip)+'_cipr.txt',sep=' ',index=False)
print("cipr save ok")

# Establish a mapping relationship between nodes and IP cores
types_list=(data[-1].strip().split(' '))*3
# print(types_list)
# types
node_list = []
for i in range(1,3*node_number+1):
    node_list.append(i)
# print(node_list)
node_ip = dict(zip(node_list,types_list))
# print(node_ip)
# 开始转换
for key in result_dict:
    each_list = []
    for each in result_dict[key]:
        each_list.append(node_ip[each])
    result_dict[key]=each_list
    
# print(result_dict)



save_dict = copy.deepcopy(result_dict)
max_len = 0
for k,v in save_dict.items():
    max_len=max(max_len,len(v))
for k,v in save_dict.items():
    if len(v)<max_len:
        for i in range(max_len-len(v)):
            v.append(-1)
save_dict

save = pd.DataFrame(save_dict)

save.to_csv('../data/graphColor/'+str(file_number)+'_'+str(n_ip)+'_cip.txt',sep=' ',index=False)
print("cip save ok")

# ## step 8 Generate weight matrix and store



# Generate weight matrix
# Read the weight data provided by the manufacturer
vendor_weight = pd.read_csv('../data/rawData/weight.csv')

s,col = vendor_weight.shape


weightMatrix = zeros((s,minimum_color),dtype=int) # the minimum number of graph coloring needed


# All are trusted IP cores, calculated directly at vendor1 price
if n_ip==len(typeSetSeq):
    totalprice=0
    for each in typeSetSeq:
        totalprice += vendor_weight[each][0] # Columns, rows Calculated at the price of vendor1
    # print(totalprice)
# Has a trusted IP core, calculated at vendor1 price
elif n_ip>0 and n_ip<len(typeSetSeq):
    frontSeq  = typeSetSeq[:n_ip]  # Trusted ip core
    # print(frontSeq)
    # Consider the price of vendor1 as the price of the trusted IP core
    for key in result_dict: # Color fill column
        i=0 
        while i<s:  # each row
            totalprice = 0
            d_set = set()
            # Color: IP core corresponding to the corresponding node
            for each in result_dict[key]:
                if each in d_set:
                    continue
                else:
                    d_set.add(each)
                    if each in frontSeq:  # Trusted ip core
                        totalprice += vendor_weight[each][0] # Columns, rows Calculated at the price of vendor1
                    else: # Is an untrusted ip core
                        totalprice += vendor_weight[each][i] # Columns, rows
            weightMatrix[i][key-1]=totalprice
            i+=1
    # print(-weightMatrix.T)
else:
    # Are untrusted IP core calculations
    for key in result_dict: # Color fill column
        i=0 
        while i<s:  # each row
            totalprice = 0
            d_set = set()
            # Color: IP core corresponding to the corresponding node
            for each in result_dict[key]:
    #             print(each)
                if each in d_set:
                    continue
                else:
                    d_set.add(each)
                    totalprice += vendor_weight[each][i] # Columns, rows
            weightMatrix[i][key-1]=totalprice
            i+=1
    # print(-weightMatrix.T)

# save weight
import numpy as np
np.savetxt("../data/graphColor/"+str(file_number)+'_'+str(n_ip)+'_weight.txt',-weightMatrix.T,fmt="%d")
print("weight save ok")



