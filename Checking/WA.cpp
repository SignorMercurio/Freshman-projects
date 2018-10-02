#include<iostream>
#include<algorithm>
using namespace std;

bool cmp(int a,int b)
{
    return a>b;
}
int main()
{
    int n;
    long long m;
    int w[18];
    cin>>n>>m;
    for(int i=0;i<n;i++) cin>>w[i];
    sort(w,w+n,cmp);
    int ans=0,all=n;
    while(all>0) {
        long long tmp=0;
        for(int i=0;i<n;i++) {
            if(!w[i]) continue;
            else if(tmp+w[i]<=m) {
                tmp+=w[i];
                w[i]=0;
                all--;
            }
        }
        ans++;
    }
    cout<<ans;
    return 0;
}
