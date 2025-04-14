#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <list>
#include <queue>
#include <math.h>
#include <time.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
struct minheap{
    int num;
    int ver;

};
struct binomialheap{
    int num,ver,deg;
    binomialheap *par,*sib,*child;
};
struct fiboheap{
    int num,ver,deg,mark;
    fiboheap *par,*child,*l,*r;
};
int mini(int arr[],int col[],int n){
    int x=999999,m;
    for(int i=0;i<n;i++){
    if(col[i]!=1&&x>arr[i]){
        m=i;
        x=arr[i];
     }
    }
    return m;
}

void arrbased(vector <int> adjmat[],int dij[],int n,int s){
    int i,col[n];//col for checking if visited or not
        for(i=0;i<n;i++){
            dij[i]=999999;
            col[i]=0;
        }
        int sz=n;
        dij[s]=0;
        while(sz>0){
            int v=mini(dij,col,n);
            col[v]=1;
            for(i=0;i<n;i++){
                if(col[i]==0&&adjmat[v][i]!=999999&&dij[i]>dij[v]+adjmat[v][i]){
                    dij[i]=dij[v]+adjmat[v][i];
                }
            }
            sz--;
        }
}
/******************binary heap*******************/
void swapheap(struct minheap heap[],int i,int j,int pos[]){
    int u=heap[i].ver,v=heap[j].ver;
    int temp=heap[i].num;
    heap[i].num=heap[j].num;
    heap[j].num=temp;
    temp=heap[i].ver;
    heap[i].ver=heap[j].ver;
    heap[j].ver=temp;
    temp=pos[u];
    pos[u]=pos[v];
    pos[v]=temp;

}
void heapify(struct minheap heap[],int n,int i,int pos[]){
    int l=2*i+1,r=2*i+2,m=i;
    if(l<n&&heap[l].num<heap[m].num){
        m=l;
    }
    if(r<n&&heap[r].num<heap[m].num){
        m=r;
    }
    if(m!=i){
        swapheap(heap,i,m,pos);
        heapify(heap,n,m,pos);
    }
}

void buildheap(struct minheap heap[],int n,int pos[]){
    int i,s=n/2-1;
    for (i = s; i >= 0; i--) {
        heapify(heap,n,i,pos);
    }
}
minheap extmin(struct minheap heap[],int n,int pos[]){
    struct minheap least=heap[0];
    swapheap(heap,0,n-1,pos);
    heapify(heap,n-1,0,pos);
    return least;
}
/*int inheap(struct minheap heap[],int sz,int v){
    int i,c=0;
    for(i=0;i<sz;i++){
       if(heap[i].ver==v){
            c=1;
            return c;
       }
    }
    return c;
}*/
void deckey(struct minheap heap[],int sz,int v,int d,int pos[]){
        int i=v;
        heap[i].num=d;
         while(i!=0&&heap[i].num<heap[(i-1)/2].num){
            swapheap(heap,i,(i-1)/2,pos);
            i=(i-1)/2;
         }

}

void heapbased(vector <int> adjmat[],vector <int> adjlist[],struct minheap heap[],int dij[],int n,int s){
    int pos[n];
    int sz=n,i;
    for(i=0;i<n;i++){
            heap[i].num=999999;
            heap[i].ver=i;
            pos[i]=i;
            dij[i]=999999;
    }
    heap[s].num=0;
    dij[s]=0;
    buildheap(heap,n,pos);

    while(sz>0){
        struct minheap least=extmin(heap,sz,pos);
        sz--;
        int v=least.ver;
        /*for(i=0;i<sz;i++)
            cout<<heap[i].num<<" "<<heap[i].ver<<" ";
        cout<<endl;*/
        //cout<<v<<" "<<least.num<<" ";
        for(i=0;i<adjlist[v].size();i++){
            int u=adjlist[v][i];
                if(pos[u]<sz&&dij[u]>dij[v]+adjmat[v][u]){
                    dij[u]=dij[v]+adjmat[v][u];
                    //cout<<dij[i]<<" "<<i;
                    deckey(heap,sz,pos[u],dij[u],pos);
                }
            }
    }
}
/***********************binomial heap********************/


list <binomialheap*> unionbheap(list<binomialheap*> bheap1,list<binomialheap*> bheap2){
 list <binomialheap*> temp;
    list <binomialheap*> :: iterator st1=bheap1.begin(),st2=bheap2.begin(),st4;
    int ct=0;
    while(st1!=bheap1.end()&&st2!=bheap2.end()){
        //cout<<"Ct "<<ct<<endl;
        //ct++;
        if((*st1)->deg<(*st2)->deg){
            temp.push_back(*st1);
            st1++;
        }
        else if((*st1)->deg>(*st2)->deg){
            temp.push_back(*st2);
            st2++;
        }
        else{
            temp.push_back(*st1);
            temp.push_back(*st2);
            st2++;
            st1++;
        }
    }
    while(st2!=bheap2.end()){
        temp.push_back(*st2);
        st2++;
    }
    while(st1!=bheap1.end()){
        temp.push_back(*st1);
        st1++;
    }
    /*st4=temp.begin();

    while(st4!=temp.end()){
        cout<<(*st4)->deg<<" ";
        st4++;
    }
    cout<<"d";*/
    return temp;
}

binomialheap *mergebheap(binomialheap *bheap1,binomialheap *bheap2){
    if(bheap2->num<bheap1->num){
    bheap1->par=bheap2;
    bheap1->sib=bheap2->child;
    bheap2->child=bheap1;
    bheap2->deg+=1;
    return bheap2;
    }
    else{
    bheap2->par=bheap1;
    bheap2->sib=bheap1->child;
    bheap1->child=bheap2;
    bheap1->deg+=1;
    return bheap1;
    }

}

binomialheap *minbheap(list <binomialheap*> binoheap){
    list <binomialheap*> :: iterator st=binoheap.begin();
    binomialheap *min=*st;
    while(st!=binoheap.end()){
        if(min->num>(*st)->num){
            min=*st;
        }
        st++;
    }
    return min;
}
list <binomialheap*> checkdeg(list<binomialheap*> binoheap){
    list <binomialheap*> :: iterator st1=binoheap.begin(),st2,st3,st4=binoheap.begin();
    st2=st1;st3=st2;
    if(binoheap.size()<=1)
      return binoheap;
    else{
        if(binoheap.size()==2){
            st2++;
            st3=st2;
            st3++;
        }
        else{
            st2++;
            //st3=st2;
            st3++;
            st3++;
        }
    }
    /*if(st3==binoheap.end())
        cout<<"yes";*/
        int ct=0;
    while(st1!=binoheap.end()){

    //cout<<(*st3)->deg;
            ct++;
            //cout<<ct<<" ";
        if(st2==binoheap.end()){
            //cout<<'!';
            //st1++;
            break;
        }
        else if((*st2)->deg>(*st1)->deg){
            //cout<<'@';
            st2++;
            st1++;
            if(st3!=binoheap.end())
                st3++;
        }
        //if all 3 r same degree then we only merge last 2
        else if(st3!=binoheap.end()&&(*st2)->deg==(*st1)->deg&&(*st2)->deg==(*st3)->deg){
            //cout<<'#';
            st1++;
            st2++;
            st3++;
        }
        else if((*st2)->deg==(*st1)->deg){//now we merge

            *st1=mergebheap(*st1,*st2);
            //cout<<'$';
             binoheap.erase(st2);
             st2=st1;
             st2++;
            if(st3!=binoheap.end())
                st3++;
        }
        //st4=binoheap.end();
    }

    return binoheap;
}

list <binomialheap*> extminb(list <binomialheap*> binoheap){
    binomialheap *temp1=minbheap(binoheap),*temp2,*temp3;
    list <binomialheap*> bheap1,bheap2;
    list <binomialheap*> :: iterator st=binoheap.begin();
    temp2=temp1->child;

    while(st!=binoheap.end()){
        if(*st!=temp1){
            bheap1.push_back(*st);
        }
        st++;
    }
    while(temp2!=NULL){
        temp3=temp2;
        temp2=temp2->sib;
        temp3->sib=NULL;
        bheap2.push_front(temp3);//push all subtrees
    }

    bheap1=unionbheap(bheap1,bheap2);
    //cout<<bheap1.size();
    bheap1=checkdeg(bheap1);

    return bheap1;
}


list <binomialheap*> add(list<binomialheap*> binoheap,binomialheap *bino[], int d,int v){
    binomialheap *node;
    node = (struct binomialheap*)malloc(sizeof(struct binomialheap));
    list <binomialheap*> temp;
    node->num=d;
    node->ver=v;
    node->deg=0;
    node->par = NULL;
    node->child = NULL;
    node->sib = NULL;
    bino[v]=node;
    temp.push_back(node);
    temp=unionbheap(temp,binoheap);
    temp=checkdeg(temp);
    return temp;
}

void deckeybheap(list<binomialheap*> binoheap,binomialheap *bino[],int v,int newdist){

    binomialheap *change=bino[v],*pare;
    change->num=newdist;
    pare=change->par;
    int temp;
    while(pare!=NULL&&change->num<pare->num){
        temp=pare->num;
        pare->num=change->num;
        change->num=temp;
        temp=pare->ver;
        pare->ver=change->ver;
        change->ver=temp;
        bino[change->ver]=change;
        bino[pare->ver]=pare;
        change=pare;
        pare=change->par;
    }
    //return binoheap;
}
void binoheapbased(vector <int> adjmat[],vector <int> adjlist[],list <binomialheap*> binoheap,int dij[],int n,int s){
    int sz=n,i,col[n];
    binomialheap *bino[n];
    for(i=0;i<n;i++){
        col[i]=0;
        dij[i]=999999;
        bino[i]=NULL;
    }
    binoheap=add(binoheap,bino,0,s);
    //cout<<bino[s]->num;
    dij[s]=0;
    while(!binoheap.empty()){

        binomialheap *least=minbheap(binoheap);
        binoheap=extminb(binoheap);

        sz--;
        int v=least->ver;
        bino[v]=NULL;
        col[v]=1;
        //cout<<v<<" "<<adjlist[v].size();
        //cout<<v<<" "<<least->num<<endl;
        for(i=0;i<adjlist[v].size();i++){
                //cout<<adjlist[v].size()<<" ";
            int u=adjlist[v][i];
                if(col[u]==0&&dij[u]>dij[v]+adjmat[v][u]){
                    dij[u]=dij[v]+adjmat[v][u];
                    //cout<<dij[u]<<" "<<u<<" "<<endl;
                    //cout<<bino[u]->num;
                    if(bino[u]==NULL){
                    binoheap=add(binoheap,bino,dij[u],u);}
                    else deckeybheap(binoheap,bino,u,dij[u]);
                }

            }

    }
}


/******************fibonacci heap*******************/
struct fiboheap *fiboadd(struct fiboheap *fibomin,fiboheap *fib[], int d,int v,int dg){
    fiboheap *node;
    node = (struct fiboheap*)malloc(sizeof(struct fiboheap));

    node->num=d;
    node->ver=v;
    node->deg=dg;
    node->mark=0;
    node->par = NULL;
    node->child = NULL;
    node->l = node;
    node->r= node;
    fib[v]=node;
    if(fibomin==NULL){
        return node;
    }
    else{
        (fibomin->l)->r=node;
        node->l=fibomin->l;
        node->r=fibomin;
        fibomin->l=node;
        fib[v]=node;
        if(fibomin->num>d)
            fibomin=node;
        return fibomin;
    }

}

void join(struct fiboheap *fh1,struct fiboheap *fh2){
     fh1->l->r = fh1->r;
     fh1->r->l = fh1->l;
     fh1->par=fh2;
     fh2->deg++;
     if(fh2->child==NULL){
        fh1->l=fh1;
        fh1->r=fh1;
        fh2->child=fh1;
     }
     else{
        fh2->child->r->l=fh1;
        fh1->r=fh2->child->r;
        fh2->child->r=fh1;
        fh1->l=fh2->child;
        if(fh2->child->num >fh1->num)
            fh2->child=fh1;
     }
    fh1->mark=0;
}


struct fiboheap *fibcons(struct fiboheap *fibomin,int sz){
    float e=1.6;
    float a=log(sz)/log(e);
    int n=ceil(a),i,c=0,f=0;
    fiboheap *arr[n],*fh1=fibomin,*fh2,*temp;
    for(i=0;i<n;i++)
        arr[i]=NULL;
    while(fh1!=fibomin&&c!=1){
        c=1;
        int d=fh1->deg;
        while(d<n&&arr[d]!=NULL){
        fh2=arr[d];
        if(fh1==fh2){//same ptr
            f=1;
            break;
        }
        if(fh2->num<fh1->num){//swap
            temp=fh2;
            fh2=fh1;
            fh1=temp;
        }
        join(fh2,fh1);//joining ptrs of same deg

        arr[d]=NULL;
        d++;
        }
        if(f==1)
        break;
        arr[fh1->deg]=fh1;
        fh1=fh1->r;
    }
    fibomin=fh1;

    temp=fibomin;
    c=0;
    while(temp!=fibomin||c!=1){
        c=1;
        if(temp->num<fibomin->num)
        fibomin=temp;
        temp=temp->r;
    }
    for(i=0;i<n;i++)
        arr[i]=NULL;
    return fibomin;
}

struct fiboheap *extminfibo(struct fiboheap *fibomin,int sz){

    fiboheap *temp1=fibomin,*temp2,*temp3;
    sz--;
    if(temp1->child!=NULL){
        temp2=temp1->child;
        temp3=temp2;//temp3 for least dist
        while(temp2->par!=NULL){
            temp2->par=NULL;
            if(temp3->num>temp2->num){
                temp3=temp2;
            }
            temp2=temp2->r;
        }
        if(temp1->l==temp1){//only one tree
            fibomin=temp3;
        }
        else{
            temp2=temp1->child->r;
            temp1->l->r=temp1->child;
            temp1->child->r=temp1->l;
            temp1->r->l=temp2;
            temp2->l=temp1->r;
        }
        if(temp3->num<fibomin->num)
        fibomin=temp3;
    }
    else{
        temp1->r->l=temp1->l;
        temp1->l->r=temp1->r;
    }
    temp1=fibomin;
    fibomin=fibomin->l;//will be fixed in fibcons
    temp1->child=NULL;
    temp1->l=NULL;
    temp1->r=NULL;
    delete temp1;
    if(sz>1)
    fibomin=fibcons(fibomin,sz);
    else if(sz==1)
    return fibomin;
    else return NULL;
    return fibomin;
}

void cut(struct fiboheap *fibomin,struct fiboheap *fh1,struct fiboheap *fh2){//remove parent and place it in fibomin list
    if(fh1->l==fh1)
        fh2->child=NULL;
    else{
        fh1->r->l=fh1->l;
        fh1->l->r=fh1->r;
        fh2->child=fh1->r;
    }
    fh1->mark=0;
    fh1->par=NULL;
    fh2->deg--;
    fh1->l=fibomin;
    fh1->r=fibomin->r;
    fibomin->r->l=fh1;
    fibomin->r=fh1;
}
 void cascut(struct fiboheap *fibomin,struct fiboheap *fh1){
     fiboheap *parent;
     if(fh1->par!=NULL){
         parent=fh1->par;
        if(fh1->mark==0)
            fh1->mark=1;
        else{
            cut(fibomin,fh1,parent);
            cascut(fibomin,parent);
        }
     }

 }

void deckeyfheap(struct fiboheap *fibomin,fiboheap *fib[],int v,int dist){

    fiboheap *change=fib[v];
    change->num=dist;

    if(change->par!=NULL&&change->num<change->par->num){
        cut(fibomin,change,change->par);
        cascut(fibomin,change);
    }
    if(change->num<fibomin->num)
        fibomin=change;
}

void fiboheapbased(vector <int> adjmat[],vector <int> adjlist[],fiboheap *fibomin,int dij[],int n,int s){
    int sz=n,i,col[n],ct=0;
    fiboheap *fib[n];
    for(i=0;i<n;i++){
        col[i]=0;
        dij[i]=999999;
        fib[i]=NULL;
    }
    fibomin=fiboadd(fibomin,fib,0,s,0);
    ct++;
    dij[s]=0;
    /*for(i=0;i<n;i++){
        if(i!=s)
        fibomin=fiboadd(fibomin,fib,999999,i,0);
    }*/
    //cout<<fib[s]->num;
    while(fibomin!=NULL){
        int v=fibomin->ver;
       // cout<<v<<" "<<fibomin->num<<" "<<ct<<endl;

        fibomin=extminfibo(fibomin,ct);
        sz--;ct--;//cout<<"ver";
        fib[v]=NULL;
        /*if(ct!=0)
        cout<<fibomin->ver<<" "<<fibomin->num<<" "<<ct<<endl;*/
        col[v]=1;
        //cout<<v<<" "<<least.num<<" ";
        for(i=0;i<adjlist[v].size();i++){
                //cout<<adjlist[v].size()<<" ";
                //cout<<'x';
            int u=adjlist[v][i];
                if(col[u]==0&&dij[u]>dij[v]+adjmat[v][u]){
                    dij[u]=dij[v]+adjmat[v][u];
                   //cout<<dij[u]<<" "<<u<<" "<<endl;
                   if(fib[u]==NULL){
                    fibomin=fiboadd(fibomin,fib,dij[u],u,0);
                    ct++;
                   }
                    else deckeyfheap(fibomin,fib,u,dij[u]);
                }
            }
    }
}
int main(int argc,char *argv[])
{
    int t,yz,choice;
    cin>>t;
    double clck[t];
    clock_t starttime,endtime;
    if(argc==1)
        choice=4;
    else choice=atoi(argv[1]);
    //test case loop
    for(yz=0;yz<t;yz++){
    int i,j,k,n,d,u,und=0,f=0;
    cin>>n>>d;
    int bellmat[n+1][n+1],dist[n+1];
    struct minheap heap[n];

    vector <int> adjmat[n];
    int x;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cin>>x;
            adjmat[i].push_back(x);
            if(d==0&&adjmat[i][j]<0)
                und=1;//negative cycle for undirected
        }
    }
    //auto starttime=high_resolution_clock::now();

    starttime=clock();
    vector <int> adjlist[n],bell[n+1];
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
                bellmat[i+1][j+1]=adjmat[i][j];
                if(adjmat[i][j]!=999999&&i!=j){
                    adjlist[i].push_back(j);
                    if(d!=0)
                    bell[i+1].push_back(j+1);
                }
            }
    }
    if(d!=0){
    for(i=0;i<=n;i++){
        bell[0].push_back(i);
        bellmat[0][i]=0;
    }
    for(i=0;i<=n;i++){
        dist[i]=999999;
    }
    dist[0]=0;
    for(i=0;i<n;i++){
        for(j=0;j<=n;j++){
            if(dist[j]!=999999){
            for(k=0;k<bell[j].size();k++){
                    u=bell[j][k];
                    if(dist[u]>dist[j]+bellmat[j][u]){
                    dist[u]=dist[j]+bellmat[j][u];
                    }
            }
            }
        }
        }
        f=0;//negative cycle
        for(j=0;j<n;j++){
                if(dist[j]!=999999){
                for(k=0;k<bell[j].size();k++){
                    u=bell[j][k];
                    if(dist[u]>dist[j]+bellmat[j][u]){
                        f=1;
                        break;
                    }
                }
                }
            if(f==1)
                break;
            }
    }
        if(f==1||und==1){
            cout<<"-1"<<endl;
        }
        else{
        if(d==1){
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                if(adjmat[i][j]!=999999)
                adjmat[i][j]+=dist[i+1]-dist[j+1];
            }
        }
        }
//bellmann ford ends
        int dij[n];
        if(choice==1){

        for(i=0;i<n;i++){
          arrbased(adjmat,dij,n,i);//array based implementation
           for(j=0;j<n;j++){
            if(dij[j]!=999999){
                    if(d==1)
                    cout<<dij[j]+dist[j+1]-dist[i+1]<<" ";
                    if(d==0)
                    cout<<dij[j]<<" ";
                }
            else cout<<dij[j]<<" ";
            }
            cout<<endl;
        }
        }
        /*for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                cout<<adjmat[i][j]<<" ";
            }
            cout<<endl;
        }*/
        else if(choice==2){

        for(i=0;i<n;i++){
            heapbased(adjmat,adjlist,heap,dij,n,i);//binary heap based implementation
            for(j=0;j<n;j++){
            if(dij[j]!=999999){
                    if(d==1)
                    cout<<dij[j]+dist[j+1]-dist[i+1]<<" ";
                    if(d==0)
                    cout<<dij[j]<<" ";
                }
            else cout<<dij[j]<<" ";
            }
            cout<<endl;
        }
        }
        else if(choice==3){

        for(i=0;i<n;i++){
            list<binomialheap*> binoheap;
            binoheapbased(adjmat,adjlist,binoheap,dij,n,i);//binomial heap based implementation
            for(j=0;j<n;j++){
            if(dij[j]!=999999){
                    if(d==1)
                    cout<<dij[j]+dist[j+1]-dist[i+1]<<" ";
                    if(d==0)
                    cout<<dij[j]<<" ";
                }
            else cout<<dij[j]<<" ";
            }
            cout<<endl;
        }
        }
        else if(choice==4){

        for(i=0;i<n;i++){
            struct fiboheap *fibomin=NULL;
            fiboheapbased(adjmat,adjlist,fibomin,dij,n,i);//fibonacci heap based implementation
            for(j=0;j<n;j++){
            if(dij[j]!=999999){
                    if(d==1)
                    cout<<dij[j]+dist[j+1]-dist[i+1]<<" ";
                    if(d==0)
                    cout<<dij[j]<<" ";
                }
            else cout<<dij[j]<<" ";
            }
            cout<<endl;
        }
        }

       // cout<<time<<endl;

    }
    endtime=clock();

    clck[yz]=double(endtime-starttime)/double(CLOCKS_PER_SEC);
    }
    for(yz=0;yz<t;yz++){
        cout<<clck[yz]<<fixed<<setprecision(5);
        cout<<" ";
    }
    return 0;
}


