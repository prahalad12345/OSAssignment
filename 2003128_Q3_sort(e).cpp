#include <iostream> 
#include <vector> 
#include <random>
using namespace std;
//here schedular is the ready queue
//the creation is done using linked list
int idd=1;

class Node{
    public: 
        int id;
        int time;
        Node *next;
        Node(int val){
            id=idd;
            time=val;
            next=NULL;
        }
        Node(int idg,int val){
            id=idg;
            time=val;
            next=NULL;
        }
};

class Schedular{
    Node *head;
    public:
        Schedular(){
            head=NULL;
        }
        Schedular(int time){
            head->time=time;
            head->next=NULL;
        }
        void push(int time);
        void push(int id,int time);
        Node* pop();
        Node* top();
        bool empty();
};

void Schedular::push(int time){
    if(head==NULL){
        head=new Node(time);
    }
    else{
        Node *newnode=new Node(time);

        Node* temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=newnode;
    }
    idd++;
}
void Schedular::push(int id,int time){
    if(head==NULL){
        head=new Node(time);
    }
    else{
        Node *newnode=new Node(id,time);
        Node* temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=newnode;
    }
}

Node* Schedular::pop(){
    if(head==NULL){
        return NULL;
    }
    else{
        Node* val=head;
        head=head->next;
        return val;
    }
}

bool Schedular::empty(){
    if(head==NULL)
        return 1;
    return 0;
}

Node* Schedular::top(){
    if(head==NULL){
        return NULL;
    }
    else{
        return head;
    }
}

void execution(Schedular &s,vector<int> &timeconsumed,vector<bool> &completed){
    while(!s.empty()){
        Node *val=s.pop();

        if(val->time<=2){
            for(int i=0;i<timeconsumed.size();i++){
                if(completed[i]){
                    timeconsumed[i]+=val->time;
                }
            }
            completed[val->id]=0;
        }
        else{
            for(int i=0;i<timeconsumed.size();i++){
                if(completed[i]){
                    timeconsumed[i]+=2;
                }
            }
            s.push(val->id,val->time-2);

        }
    }
}

int main(){
    Schedular s;
    //created
    vector<int> time_consumed(25,0);
    vector<bool> completed(25,1);
    vector<int> times;
    for(int i=0;i<25;i++){
        int timepush=(i*100+98)%7;
        if(timepush<2){
            timepush+=2;
        }
        times.push_back(timepush);
    }
    sort(times.begin(),times.end());
    for(int i=0;i<times.size();i++){
        cout<<times[i]<<endl;
        s.push(times[i]);
    }

    execution(s,time_consumed,completed);
    for(int i=0;i<time_consumed.size();i++){
        cout<<i+1<<","<<time_consumed[i]<<endl;
    }
}