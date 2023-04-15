#include<iostream>

using namespace std;

struct Skill{
        int skill_ID;
        float proficiency;
};

struct Resource{
    int id;
    bool availability;
    int res_num;
    int res_app;
    bool in_use;
    Skill skills;
};

struct task{
    int id;
    int dur;
    int early_start;
    int early_finish;
    int late_start;
    int late_finish;
    int *depend;
    int *rev_depend;
    int depend_amount;
    int reverse_count;
    bool completion;
    int slack;
    int res_req;
    Resource r;
};

struct project{
    int id;
    int t;
    int max;
    int task_comp;
    task * tasks;
    Resource * resc;
    int resc_in_use;

};

void setTaskDuration(project &p, int t)
{
    for(int i=0; i<t; i++)
    {
        cout<<"Enter time duration of task id "<<p.tasks[i].id<<": ";
        cin >> p.tasks[i].dur;
    }

    cout<<endl;
}


void set_nth_TaskDuration(project &p, int t, int option)
{
    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].id == option)
        {
            cout<<"Enter time duration for task id "<<p.tasks[i].id<<": ";
            cin>>p.tasks[i].dur;
            break;
        }
    }

    cout<<endl;
}

void printTaskDependencyList(project &p, int t, int option)
{
    for(int i=0; i<t; i++)
    {
        //if(option==p.tasks[i].id)
        {
            cout<<"Task id "<<p.tasks[i].id<<" "<<"Dependencies:  ";

            for(int j=0; j<p.tasks[i].depend_amount; j++)
            {
                cout<<p.tasks[i].depend[j]<<"\t";
            }

            cout<<endl;
        }
    }

    cout<<endl;

   for(int i=0; i<t; i++)
   {
       //if(option==p.tasks[i].id)
       {
           cout<<"Task id "<<p.tasks[i].id<<" "<<"Reverse Dependencies: ";

           for(int j=0; j<p.tasks[i].reverse_count; j++)
           {
               cout<<p.tasks[i].rev_depend[j]<<"\t";
           }

          cout<<endl;
        }
   }

}

void addtasks(project &p, int t)
{
    for(int i=0; i<t; i++)
    {
        p.tasks[i].id=i;
        p.tasks[i].rev_depend = new int[t];
        p.tasks[i].reverse_count=0;
        p.tasks[i].completion=false;
        p.tasks[i].r.availability=false;
    }

    setTaskDuration(p, t);

    for(int i=0; i<t; i++)
    {
        do{
            cout<<"Task id "<< p.tasks[i].id <<" is dependent on how many tasks: ";
            cin>>p.tasks[i].depend_amount;
        }while(!(p.tasks[i].depend_amount>=0));

        p.tasks[i].depend = new int[p.tasks[i].depend_amount];

        for(int j=0; j<p.tasks[i].depend_amount; j++)
        {
            cout<<"Task id "<<p.tasks[i].id<<" is dependant on number "<<j+1<<", task id: ";
            cin>>p.tasks[i].depend[j];

            int reverse=p.tasks[p.tasks[i].depend[j]].reverse_count;

            p.tasks[p.tasks[i].depend[j]].rev_depend[reverse] = p.tasks[i].id;

            reverse++;

            p.tasks[p.tasks[i].depend[j]].reverse_count=reverse;
        }

        cout<<endl;
    }
}

void early(project &p, int t, int i,bool flag)
{
    if(i==t)
       return;

    int total=0;

    if(p.tasks[i].completion==false)
    {
        for(int j=0; j<p.tasks[i].depend_amount; j++)
        {
            int ind = p.tasks[i].depend[j];

            if(p.tasks[ind].completion==false)
            {
                early(p,t,ind,false);
            }

            if(total < p.tasks[ind].early_finish)
            {
                total = p.tasks[ind].early_finish;
            }
        }

         p.tasks[i].early_start = total;
         p.tasks[i].early_finish = total + p.tasks[i].dur;
         p.tasks[i].completion=true;

    }

    if(flag==true)
        early(p,t,++i,true);

    return;
}

void late(project &p, int t, int i,bool flag)
{
    if(i==t)
       return;

    int total=p.max;

    if(p.tasks[i].completion==false)
    {
        for(int j=0; j<p.tasks[i].reverse_count; j++)
        {
            int ind = p.tasks[i].rev_depend[j];

            if(p.tasks[ind].completion==false)
            {
                late(p,t,ind,false);
            }

            if(total > p.tasks[ind].late_start)
            {
                total = p.tasks[ind].late_start;
            }
        }

         p.tasks[i].late_finish = total;
         p.tasks[i].late_start = total - p.tasks[i].dur;
         p.tasks[i].completion=true;

    }

    if(flag==true)
        late(p,t,++i,true);

    return;
}

void calculateBasicSchedule(project &p, int t)
{
    early(p,t,0,true);

    for(int i=0; i<t; i++)
    {
        p.tasks[i].completion=false;
    }

    p.max = 0;

    for(int i=0; i<t; i++)
    {
        if(p.max < p.tasks[i].early_finish)
        {
            p.max = p.tasks[i].early_finish;
        }
    }

    late(p,t,0,true);

    int late_start=p.max;

    for(int i=0; i<t; i++)
    {
        if(late_start > p.tasks[i].late_start)
        {
            late_start = p.tasks[i].late_start;
        }
    }

    for(int i=0; i<t; i++)
    {
        p.tasks[i].slack= abs((p.tasks[i].early_start) - (p.tasks[i].late_start));

    }

    cout<<endl<<"Basic schedule:"<<endl<<"ID\t"<<"ES\t"<<"EF\t"<<"LS\t"<<"LF\t"<<"Slac"<<endl;
    for(int i=0; i<t; i++)
    {
        cout<<i<<"\t"<< p.tasks[i].early_start<<"\t"<<p.tasks[i].early_finish<<"\t"<<p.tasks[i].late_start<<"\t"<<p.tasks[i].late_finish<<"\t"<<p.tasks[i].slack<<endl;
    }

    cout<<endl<<"Early finish time is: "<< p.max <<endl;
    cout<<"Late start time is: "<<late_start<<endl;
}

void printCriticalTasks(project &p, int t)
{
    cout<<endl;

    for(int i=0; i<t; i++)
    {
        if( p.tasks[i].early_start == p.tasks[i].late_start || p.tasks[i].early_finish == p.tasks[i].late_finish)
        {
            cout<<"Task id "<<p.tasks[i].id<<" is critical task."<<endl;
        }
    }

    cout<<endl;
}

void res_alloc(project &p, int t, int n, int day, int k)
{
    if(k==n)
      return;

    if(p.resc[k].availability==true)
    {
        int index;

        for(int i=0; i<t; i++)
        {
            if(p.tasks[i].r.availability==false)
            {
                index=i;
            }
        }

        bool flag2=false;

        for(int i=0; i<t; i++)
        {
            if(p.tasks[i].r.availability==false)
            {
                bool flag =true;

                for(int j=0; j<p.tasks[i].depend_amount; j++)
                {
                    int ind = p.tasks[i].depend[j];

                    if(p.tasks[ind].completion==false)
                    {
                        flag=false;
                        break;
                    }
                }

                if(flag==true)
                {
                    if(p.tasks[i].late_start< p.tasks[index].late_start)
                    {
                        index=i;
                    }

                    /*else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack<p.tasks[index].slack)
                    {
                        index=i;
                    }*/

                    /*
                    else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack==p.tasks[index].slack && p.tasks[i].dur<p.tasks[index].dur)
                    {
                        index=i;
                    }*/

                    flag2=true;
                }
            }
        }

        if(flag2==true)
        {
            p.tasks[index].r.id=p.resc[k].id;
            p.resc[k].availability=false;
            p.tasks[index].r.availability=true;
            p.resc_in_use++;
            p.tasks[index].early_start=day;
        }
    }

    res_alloc(p,t,n,day,++k);
}

void res_sch(project &p, int t, int n, int day)
{
    if(p.task_comp==t)
      return;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==true && p.tasks[i].completion==false)
        {
            p.tasks[i].early_finish=day;

            if((p.tasks[i].early_finish - p.tasks[i].early_start)==p.tasks[i].dur)
            {
                p.resc[p.tasks[i].r.id].availability=true;
                p.tasks[i].completion=true;
                p.task_comp++;
                p.resc_in_use--;
            }
        }
    }

    if(p.resc_in_use!=n)
      res_alloc(p,t,n,day,0);

    res_sch(p,t,n,++day);

}

/*void res_alloc(project &p, int t, int n, int day, int k)
{
    if(k==n)
      return;

    if(p.resc[k].availability==true)
    {

    int index;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==false)
        {
            index=i;
        }
    }

    bool flag2=false;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==false)
        {

        //cout<<i<<endl;
        bool flag =true;

        //cout<<"hi"<<endl;
        for(int j=0; j<p.tasks[i].depend_amount; j++)
        {
            int ind = p.tasks[i].depend[j];

            if(p.tasks[ind].completion==false)
            {
                flag=false;
                //cout<<"hi"<<endl;
                break;
            }
        }

        //cout<<flag<<endl;
        if(flag==true)
        {
            if(p.tasks[i].late_start< p.tasks[index].late_start)
            {
                index=i;
            }

            /*else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack<p.tasks[index].slack)
            {
                index=i;
            }
            /*
            else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack==p.tasks[index].slack && p.tasks[i].dur<p.tasks[index].dur)
            {
                index=i;
            }


            //else index=i;
            flag2=true;
            //cout<<index<<endl;
        }

        //cout<<index<<endl;

        }
    }

   if(flag2==true)
    {
    p.tasks[index].r.id=p.resc[k].id;
    p.tasks[index].r.skills.proficiency=p.resc[k].skills.proficiency;
    p.resc[k].availability=false;
    p.tasks[index].r.availability=true;
    p.resc_in_use++;
    p.tasks[index].early_start=day;
    //cout<<index<<" "<<k<<" "<<flag2<<" "<<p.resc[k].availability<<endl;
    }
    }

    res_alloc(p,t,n,day,++k);

}

void res_sch(project &p, int t, int n, int day)
{
    if(p.task_comp==t)
      return;

    //cout<<endl;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==true && p.tasks[i].completion==false)
        {
            p.tasks[i].early_finish=day;

            int x= p.tasks[i].dur*(1/p.tasks[i].r.skills.proficiency);
            float y= p.tasks[i].dur*(1/p.tasks[i].r.skills.proficiency) - x;

        if((p.tasks[i].early_finish - p.tasks[i].early_start)==(x+ (y>0)))
        {
            //cout<<p.tasks[i].dur*(1/p.tasks[i].r.skills.proficiency)<<endl;
            //p.tasks[i].r.availability= false;
            p.resc[p.tasks[i].r.id].availability=true;
            p.tasks[i].completion=true;
            p.task_comp++;
            p.resc_in_use--;
        }

        }
    }

    if(p.resc_in_use!=n)
      res_alloc(p,t,n,day,0);

    res_sch(p,t,n,++day);

}*/

/*void res_alloc(project &p, int t, int n, int day, int k)
{
    if(k==n)
      return;

    for(int a=0; a<p.resc[k].res_num; a++)
    {
    if(p.resc[k].availability==true)
    {

    int index;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==false && p.resc[k].skills.skill_ID==p.tasks[i].r.skills.skill_ID)
        {
            index=i;
        }
    }

    bool flag2=false;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==false && p.resc[k].skills.skill_ID==p.tasks[i].r.skills.skill_ID)
        {

        //cout<<i<<endl;
        bool flag =true;

        //cout<<"hi"<<endl;
        for(int j=0; j<p.tasks[i].depend_amount; j++)
        {
            int ind = p.tasks[i].depend[j];

            if(p.tasks[ind].completion==false)
            {
                flag=false;
                //cout<<"hi"<<endl;
                break;
            }
        }

        //cout<<flag<<endl;
        if(flag==true)
        {
            if(p.tasks[i].late_start< p.tasks[index].late_start)
            {
                index=i;
            }

            /*else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack<p.tasks[index].slack)
            {
                index=i;
            }
            /*
            else if(p.tasks[i].late_start==p.tasks[index].late_start && p.tasks[i].slack==p.tasks[index].slack && p.tasks[i].dur<p.tasks[index].dur)
            {
                index=i;
            }


            //else index=i;
            flag2=true;
            //cout<<index<<endl;
        }

        //cout<<index<<endl;

        }
    }

   if(flag2==true)
    {
    p.tasks[index].r.id=p.resc[k].id;
    p.resc[k].res_app++;

    if(p.resc[k].res_num==p.resc[k].res_app)
    {
        p.resc[k].availability=false;
        p.resc_in_use++;
    }

    if(p.resc[k].res_app==p.tasks[index].res_req)
    {
        p.tasks[index].r.availability=true;

        p.tasks[index].early_start=day;
    }

    //cout<<index<<" "<<k<<" "<<flag2<<" "<<p.resc[k].availability<<endl;
    }
    }
    }

    res_alloc(p,t,n,day,++k);

}

void res_sch(project &p, int t, int n, int day)
{
    if(p.task_comp==t)
      return;

    //cout<<endl;

    for(int i=0; i<t; i++)
    {
        if(p.tasks[i].r.availability==true && p.tasks[i].completion==false)
        {
            p.tasks[i].early_finish=day;

        if((p.tasks[i].early_finish - p.tasks[i].early_start)==p.tasks[i].dur)
        {
            //p.tasks[i].r.availability= false;
            p.tasks[i].completion=true;
            p.task_comp++;
            p.resc[p.tasks[i].r.id].res_app= p.resc[p.tasks[i].r.id].res_app- p.tasks[i].res_req;
            if(p.resc[p.tasks[i].r.id].availability==false)
            {
                p.resc[p.tasks[i].r.id].availability=true;
                p.resc_in_use--;
            }
        }

        }
    }

    if(p.resc_in_use!=n)
      res_alloc(p,t,n,day,0);

    res_sch(p,t,n,++day);

}*/

int main()
{
    project p;
    p.id=1;

    int t,n;

    cout<<"Enter resources: ";
    cin>>n;
    p.resc= new Resource[n];

    for(int i=0; i<n; i++)
    {
        p.resc[i].id= i;
        p.resc[i].availability=true;
        p.resc[i].skills.proficiency=1;

        cout<<"Skill for res "<<p.resc[i].id<<": ";
        cin>>p.resc[i].skills.skill_ID;

        cout<<"Amount for res "<<p.resc[i].id<<": ";
        cin>>p.resc[i].res_num;

        p.resc[i].res_app=0;

        cout<<endl;
    }

    cout<<"Enter tasks: ";
    cin>>t;
    p.tasks =new task[t];

    p.task_comp=0;
    p.resc_in_use=0;

    addtasks(p,t);

    printTaskDependencyList(p,t,1);

    calculateBasicSchedule(p,t);

    printCriticalTasks(p,t);

    /*cout<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<"Skill req for task "<<p.tasks[i].id<<": ";
        cin>>p.tasks[i].r.skills.skill_ID;

        cout<<"Res req for task "<<p.tasks[i].id<<": ";
        cin>>p.tasks[i].res_req;
    }*/

    for(int i=0; i<t; i++)
    {
        p.tasks[i].completion=false;
    }

    res_sch(p,t,n,0);

    cout<<"Resource schedule:"<<endl<<"ID\t"<<"ES\t"<<"EF\t"<<"Slac\t"<<"Res"<<endl;
    for(int i=0; i<t; i++)
    {
        cout<<i<<"\t"<< p.tasks[i].early_start<<"\t"<<p.tasks[i].early_finish<<"\t"<<p.tasks[i].slack<<"\t"<<p.tasks[i].r.id<<endl;
    }

    cout<<endl;


     for(int i=0; i<t; i++)
     {
            delete[] p.tasks[i].rev_depend;
            delete[] p.tasks[i].depend;
     }

    delete[] p.tasks;
    delete[] p.resc;

    return 0;
}
