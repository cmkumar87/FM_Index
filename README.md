This is a C++ library for fm index wrote to index compressed gene sequence files. 

Due to large file sizes gene sequence files need to be compressed before storage. However, searching for subsequences within the files 
requires the compressed file to be decomrpessed. Decompression is costly in time which renders search post decompression infeasible. This 
necessitates a solution that allows search without decompression. FM index allows gene sequences to be indexed and searched without 
decompression.
