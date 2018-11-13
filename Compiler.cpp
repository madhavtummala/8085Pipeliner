#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

using namespace std;

#define pb push_back
#define p pair<int,int>
#define mp make_pair
#define MAX 10000

//A class to define a Dependency graph
class Dependency
{
    int n;                    //no of instruction
    list<int> *adj;     
    vector<int> indegree;

public:
	int **graph;              //matrix rep of graph
    Dependency(int n);
    void addEdge(int v, int w, int c);
    int putNOPs(vector<int> &res, vector<p> &spare);
    void Find(vector<int> &ans);
    void Optimum(int **arr, vector<int> size, vector<int> &ans, vector<p> &spare);
    void print();
};


//custom compare function used in sorting pairs
bool mycomp(const p &a, const p &b)
{
    return a.second<b.second;
}

/*
Given an instruction 'inst', this function calculates
the registers it used and updated.
Returns - true if memory instruction
*/
int AnalyseInstruction(string inst, int &used1, int &used2, int &updated)
{
    stringstream s(inst);
    string token;
    getline(s,token,' ');

    if(token=="STORE")
    {
        getline(s,token,' ');
        used1 = stoi(token.substr(1));

        getline(s,token,'(');
        getline(s,token,')');
        used2 = stoi(token.substr(1));
        return true;
    }

    else if(token=="MOV")
    {
        getline(s,token,' ');
        updated = stoi(token.substr(1));

        getline(s,token,' ');
        used1 = stoi(token.substr(1));

    }

    else if(token=="LOAD")
    {
        getline(s,token,' ');
        updated = stoi(token.substr(1));

        getline(s,token,'(');
        getline(s,token,')');
        used1 = stoi(token.substr(1));
        return true;
    }

    else if(token=="ADD" || token=="SUB" || token=="OR")
    {
        getline(s,token,' ');
        updated = stoi(token.substr(1));

        getline(s,token,' ');
        used1 = stoi(token.substr(1));

        getline(s,token,' ');
        if(token[0]!='#')
            used2 = stoi(token.substr(1));
    }

    return false;
}

//Constructor 
Dependency::Dependency(int n)
{
    this->n=n;
    adj = new list<int>[n];
    for(int i=0; i<n; i++)
        indegree.pb(0);

    graph = new int*[n];
    for(int i=0; i<n; i++)
        graph[i]=new int[n];

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            graph[i][j]=0;
}

//To add a new edge to dependency graph
void Dependency::addEdge(int v, int w, int c)
{
    if(v!=w)
    {
        adj[v].pb(w);
        indegree[w]++;
        if(c>graph[v][w]) 
            graph[v][w]=c;
    }
}

//To print dependency graph
void Dependency::print()
{
    list<int>::iterator it;
    for(int i=0; i<n; i++)
    {
        cout<<i+1<<" ->";
        for(it=adj[i].begin(); it!=adj[i].end(); ++it)
            cout<<" "<<*it+1;
        cout<<endl;
    }
}

/*
This function takes an optimal sequence generated
and spare instructions availiable to create a complete 
sequence of instructions by adding NOPs wherever required and 
allocating spare instructions
*/
int Dependency::putNOPs(vector<int> &res, vector<p> &spare)
{
    // for(int i=0;i<res.size();i++)
    //     cout<<res[i]<<" ";
    // cout<<endl;

    int i=0,j=0,k=0,x=0;

    // for(i=0;i<n;i++)
    // {
    // 	for(j=0;j<n;j++)
    // 		cout<<graph[i][j]<<" ";
    // 	cout<<endl;
    // }
    // cout<<endl;

    for(x=2; x<=4; x++)
    {
        for(i=0; i<=res.size()-x; i++)
        {
            if(res[i]==0) continue;

            for(j=i; j<i+x; j++)
            {
                if(res[j]==0) continue;
                //printf("Testing for %d %d\n",res[i],res[j]);

                if(graph[res[i]-1][res[j]-1]==1)
                {
                    for(k=(3+i-j); k>=0; k--)
                        res.insert(res.begin()+j,0);
                    break;
                }

            }
        }
    }

    for(int i=0;i<spare.size();i++)
    {
        bool flag=false;
        for(int j=res.size()-1;res[j]!=spare[i].second;j--)
        {
            if(res[j]==0)
            {
                flag=true;
                res[j]=spare[i].first;
                break;
            }
        }
        if(!flag) res.pb(spare[i].first);
    }

    return res.size();
}

/*Generate optimum sequence from the Dependency graph
which requires least no of NOP's
Polynomial Time and Deterministic.
*/
void Dependency::Optimum(int **arr, vector<int> size, vector<int> &ans, vector<p> &spare)
{
    int s=size.size();

    int *sum = new int[n];

    for(int i=0;i<n;i++)
    {
        sum[i]=0;
        for(int j=0;j<n;j++)
            sum[i]+=graph[j][i];
    }
    
    p *a = new p[n]; 
    int temp;

    for(int i=s-1;i>=0;i--)
    {
        int j,x=0;
        for(j=0;j<size[i];j++)
        {
            temp=0;
            if(adj[arr[i][j]-1].size()==0)
            {
                int k;
                for(k=0;k<n;k++)
                    if(graph[k][arr[i][j]-1])
                        break;
                spare.pb(mp(arr[i][j],k+1));
                continue;
            }
            if(sum[arr[i][j]-1]==-1)
                temp-=MAX;

            temp+=sum[arr[i][j]-1];

            if(i!=s-1)
            {
                int k;
                for(k=0;k<size[i+1];k++)
                    if(graph[arr[i][j]-1][arr[i+1][k]-1]==1)
                        break;
                temp+=10*k;
            }

            //printf("%d %d %d\n",arr[i][j],sum[arr[i][j]-1],temp);
            a[x++]=mp(arr[i][j],temp);
        }

        sort(a,a+x,mycomp);
        size[i]=x;

        for(int j=0;j<size[i];j++)
            arr[i][j]=a[j].first;
    }

    for(int i=0;i<s;i++)
        for(int j=0;j<size[i];j++)
            ans.pb(arr[i][j]);
}

//A function to break Dependency graph to sets
//various levels of dependencies
void Dependency::Find(vector<int> &ans)
{
    // for(int i=0;i<n;i++)
    //     cout<<indegree[i]<<" ";
    // cout<<endl;

    int **arr = new int*[n];
    for(int i=0;i<n;i++) 
        arr[i]=new int[n];

    bool *visited = new bool[n];
    for(int i=0;i<n;i++) 
        visited[i]=false;

    vector<int> size;

    int k=0,j=0; 
    bool flag=true;

    while(1)
    {  
        flag=false;
        k=0;
        for(int i=0;i<n;i++)
        {
            if(indegree[i]==0 && !visited[i])
            {
                flag=true;

                arr[j][k++]=i+1;
                visited[i]=true;
            }
        }

        list<int>::iterator it;
        for(int x=0;x<k;x++)
            for (it=adj[arr[j][x]-1].begin();it!=adj[arr[j][x]-1].end(); ++it)
                indegree[*it]--;

        if(!flag) break;
        size.pb(k);
        j++;
    }

    // printf("Printing the Set Array\n");
    // for(int i=0;i<size.size();i++)
    // {
    //     for(int j=0;j<size[i];j++)
    //     {
    //         cout<<arr[i][j]<<" "; 
    //     }
    //     cout<<endl;
    // }
    // cout<<endl;

    vector<p> spare;
    Optimum(arr,size,ans,spare);
    putNOPs(ans,spare);
}


int main()
{
	//Take Input code from a file
    string input_file = "Tests/test1.asm";
    string output_file = "Tests/test_out.asm";

    ifstream file(input_file);

    if(file.fail()){
        printf("File \"%s\" not found\n",input_file.c_str());
        return 0;
    }

    vector< string > code;
    string line;
    while(getline(file,line))
        code.pb(line);

    int n = code.size()-1;

    // printf("\nInput Code:\n");
    // for(int i=0;i<code.size();i++)
    // 	cout<<code[i]<<endl;
    // cout<<endl;

    Dependency d(n);

    //A Hash Map for availiable registers
    int Registers[33][2];
    for(int i=0; i<=32; i++) {
        Registers[i][0]=-1;
        Registers[i][1]=-1;
    }

    //Analyse each instruction for dependencies
    //Printing Dependencies and generating Graph
    //printf("Finding Dependencies...\n\n");
    int mem=0;
    for(int i=0; i<n; i++)
    {
        int used1=0,used2=0,updated=0;

        if(AnalyseInstruction(code[i],used1,used2,updated))
        {
            if(code[i+1]=="NOP" || i==n-1) mem+=4;
            else mem+=6;
        }
        //printf("%d %d %d\n",used1,used2,updated);

        if(used1)
        {
            if(Registers[used1][1]!=-1){
                d.addEdge(Registers[used1][1],i,1);
                //printf("Dependency :: line %d w.r.t line %d :: R%d\n",i+1,Registers[used1][1]+1,used1);
            }
            Registers[used1][0]=i;            
        }

        if(used2) 
        {
            if(Registers[used2][1]!=-1){
                d.addEdge(Registers[used2][1],i,1);
                //printf("Dependency :: line %d w.r.t line %d :: R%d\n",i+1,Registers[used2][1]+1,used2);
            }
            Registers[used2][0]=i;
        }

        if(updated)
        {
            if(Registers[updated][0]!=-1){
                d.addEdge(Registers[updated][0],i,-1);
                //printf("Dependency :: line %d w.r.t line %d :: R%d\n",i+1,Registers[updated][0]+1,updated);
            }
            Registers[updated][1]=i;
        }

    }

    vector<int> ans;

    //Print the Dependency graph
    // printf("\nDependency Graph:\n");
    // d.print();
    // cout<<endl;

    //Now, find out the optimal sequence without any dependencies
    d.Find(ans);

    // for(int i=0;i<ans.size();i++)
    //     cout<<ans[i]<<" ";
    // cout<<endl;

    vector< string > new_code;

    for(int i=0; i<ans.size(); i++)
    {
        if(ans[i]==0) new_code.pb("NOP");
        else new_code.pb(code[ans[i]-1]);
    }
    new_code.pb("HLT");

    // printf("\nNew Code:\n");
    // for(int i=0;i<new_code.size();i++)
    // 	cout<<new_code[i]<<endl;

    //Write the new code to a file
    printf("\nNo of Clock Cycles Wasted due to memory delay = %d\n",mem);
    printf("\nWriting the best possible code to 'test_out.txt'\n(without Dependency offering exactly the same functionality)!\n");
    
    ofstream outfile;
    outfile.open(output_file);

    if(outfile.fail()){
        printf("Output file \"%s\" couldnot be created\n",output_file.c_str());
        return 0;
    }

    for(int i=0; i<new_code.size(); i++)
        outfile<<new_code[i]<<endl;
    outfile.close();

    return 0;
}