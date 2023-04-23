#include<bits/stdc++.h>
using namespace std;
struct ljb
{
	int y;
	int ne;
}e[10003];
int sum,v[20003];
void add(int x,int y)
{
	sum++;
	e[sum].y=y;
	e[sum].ne=v[x];
	v[x]=sum;
}
int n,m,ans[10003],x,y,b[10003];
int dfs(int now,int fa){
	int dep=100000;
	if(e[v[now]].ne==0&&e[v[now]].y==fa)return 1;
	for (int i=v[now];i;i=e[i].ne)
	{
		if(e[i].y==fa||b[e[i].y])continue;
		int t=dfs(e[i].y,now);
		if(t>100000)continue;
		if(t<dep||(t==dep&&ans[now]>e[i].y)){
			dep=t;
			ans[now]=e[i].y;
		}
	}
	return dep+1;
}
int main()
{
	scanf("%d %d",&n,&m);
	for(int i=1;i<=m;++i){
		scanf("%d %d",&x,&y);
		add(x,y);
		add(y,x);
	}
	scanf("%d",&m);
	for(int i=1;i<=m;++i){
		scanf("%d",&x);
		b[x]=1;
	}
	memset(ans,-1,sizeof(ans));
	dfs(0,0);
	if(ans[0]>100000){
		printf("NULL\n");
		return 0;
	}
	printf("0");
	m=ans[0];
	while(m!=-1){
		printf("->%d",m);
		m=ans[m];
	}
	return 0;
}
