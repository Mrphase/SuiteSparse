#!/bin/bash  

# for i in email-Eu-core.txt  Wiki-Vote.txt test-5.5kEdges.txt   p2p-Gnutella31.txt  soc-pokec-relationships.txt  com-orkut.ungraph.txt com-lj.ungraph.txt  Email-EuAll.txt
# do
# for j in 1 2 4 8 16 32 56 #threads
# do
#   ./PDFS.out $i $j >> result.txt
#   #echo $result
# done
# done
./head > result.txt

#./Zehui_TR_SuiteSparse <  out_as-22july06.mtx >> result.txt
#./Zehui_TR_SuiteSparse  < out_p2p-Gnutella09.mtx >> result.txt
./Zehui_TR_SuiteSparse < out_delaunay_n13.mtx >> result.txt
./Zehui_TR_SuiteSparse < out_delaunay_n15.mtx >> result.txt
./Zehui_TR_SuiteSparse <  out_as-22july06.mtx >> result.txt
./Zehui_TR_SuiteSparse  < out_p2p-Gnutella09.mtx >> result.txt
./Zehui_TR_SuiteSparse < out_delaunay_n17.mtx >> result.txt