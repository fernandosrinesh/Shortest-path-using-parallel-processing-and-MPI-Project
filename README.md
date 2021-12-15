# Shortest path using parallel processing and MPI Project


Parallel shortest path algorithm (graph algorithms) using matrix representation of the graph and matrix multiplication.

**Shortest path using parallel processing and MPI**

**Problem**

**Parallel shortest path algorithm (graph algorithms) using matrix representation of the graph and matrix multiplication.**

**Report Content:**

**- Using matrix graph modelling.**

**- Pseudocode programming**

**- implement parallel processing in function**

**- Testing**

**- Conclusion**

**1.Using graph model**

![](RackMultipart20211215-4-64muaw_html_6ed281fdc23114bd.gif)

I used this graph model, and I converted this to 6\*6 matrix below. And I read from file, and file name is input.txt.

**6**

**0 3 5 0 7 0**

**3 0 0 2 4 0**

**5 0 0 7 4 0**

**0 2 7 0 0 5**

**7 4 4 0 0 8**

**0 0 0 5 8 0**

**2. pseudocode programming code**

pseudocode function gets shortest path using parallel processing and pseudocode method.

this is pseudocode algorithm implement using recursion sub function. using graph matrix, implement pseudocode.

in first, using matrix, find possible path step by step, and increase step 1.

and then solve path number and possible direction.

this function has 3 subs.

- if key is over node, function will exit.

- if key is the same code, compare minimum cost and output possible paths.

- if key is less than node, implement pseudocode.

the code can be found below

int path\_find(intk,intlength, stringpath) {

if(k\&gt;node-1) return 0;

if (k == node-1) {

cout\&lt;\&lt;path\&lt;\&lt;&quot; weight: &quot;\&lt;\&lt;length\&lt;\&lt;endl;

if (min \&gt; length) {

min=length;

short\_path=path;

}

}

if (k \&lt; node-1) {

#pragma omp parallel for

for (int i = 0;i \&lt; node;i++) {

if (mat[k][i] != 0) {

if (flag[i] == false) {

flag[i]=true;

char no[3];

\_itoa\_s(i+1,no,10);

string sno(no);

path\_find(i,length+mat[k][i],path+sno+&quot; &quot;);

flag[i]=false;

}

}

}

}

}

also this function can implement in another way pseudocode

int max\_sum=0,s=0;

for(int i=0;i\&lt;node;i++){

for(int j=0;j\&lt;node;j++){

if(i!=j &amp;&amp; matrix[i][j]!=0) s+=matrix[i][j];

break;

}

if(max\_sum\&lt;s){

max\_sum=s;

matrix[i][j]=s+matrix[i][j];

}

else{

break;

goto1;

}

}

**3. implement parallel processing function call**

parallel processing can be implemented 2 ways:

1) mpi

2) omp

I used both methods, using \&lt;mpi.h\&gt; and \&lt;omp.h\&gt;.

the code can be found below:

int main(intargc, char \*argv[])

{

int comm\_sz;

int my\_rank;

int comm\_sz;

MPI\_Init(&amp;argc,&amp;argv);

MPI\_Comm\_size(MPI\_COMM\_WORLD, &amp;comm\_sz);

MPI\_Comm\_rank(MPI\_COMM\_WORLD, &amp;my\_rank);

fstream fp;

fp.open(&quot;input.txt&quot;,ios::in);

fp\&gt;\&gt;node;

mat=(int\*\*)malloc(sizeof(int\*)\*node);

flag=(bool\*)malloc(sizeof(bool)\*node);

for (int i = 0;i \&lt; node;i++) {

mat[i]=(int\*)malloc(sizeof(int)\*node);

}

for (int i = 0;i \&lt; node;i++) {

flag[i]=false;

for (int j = 0;j \&lt; node;j++) {

fp\&gt;\&gt;mat[i][j];

}

}

flag[0]=true;

path\_find(0,0,&quot;1 &quot;);

cout\&lt;\&lt;&quot;----------------------------&quot;\&lt;\&lt;endl;

cout\&lt;\&lt;&quot;shortest path : &quot;\&lt;\&lt;short\_path\&lt;\&lt;&quot; : weight &quot;\&lt;\&lt;min\&lt;\&lt;endl;

return 0;

}

**4.Testing**

Using upper graph, and converted matrix, shortest path weight is 10 and possible paths are 11 ways using visual studio 2015, made program.

![](RackMultipart20211215-4-64muaw_html_f7f8106d1c2681fa.gif)

**5. conclusion**

I would like to say the mpi and omp are good parallel processing method and know well about graph data structure and pseudocode algorithm.

Note: All the necessary information available in the HPC_Report.docx
