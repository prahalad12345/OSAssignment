#include <iostream> 
#include <vector> 
#include <random>
using namespace std;
//here schedular is the ready queue
//the creation is done using linked list
int idd=1;

/*
Node consist of 
time 
task id 
and the neext pointer of the queue
and construtctors are created to access the same

*/
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

/*
ready qeueu
consiste of the head pointer 
created function for pushing ina node
popping out a node 
and extractteh value int he top of the queeueu
*/
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
//pushes the value in the queeu 
void Schedular::push(int time){
    if(head==NULL){
        head=new Node(time);
    }
    else{
        //goes to the end of the qeueu and pushes it in
        Node *newnode=new Node(time);

        Node* temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=newnode;
    }
    //increment the id
    idd++;
}
void Schedular::push(int id,int time){
    //takes id and tiemm as input and creates a node and pushes it in 
    //to the qeueu
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
    //extracts the first value in the queeue 
    //and pop it out
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
    //checks if the queeu is empty or not
    if(head==NULL)
        return 1;
    return 0;
}

Node* Schedular::top(){
    //extracts the first node of the qeueue
    if(head==NULL){
        return NULL;
    }
    else{
        return head;
    }
}

void execution(Schedular &s,vector<int> &timeconsumed,vector<bool> &completed){
    //performs all teh computtion
    while(!s.empty()){
        Node *val=s.pop();
    //if triem is less tahtn 2 perform the computation and dont push it to the queeu 
    //else perform computation and push it back to the qeueu
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
    for(int i=0;i<25;i++){
        int timepush=(i*100+98)%7;
        if(timepush<2){
            timepush+=2;
        }
        s.push(timepush);
    }

    execution(s,time_consumed,completed);
    for(int i=0;i<time_consumed.size();i++){
        cout<<i+1<<","<<time_consumed[i]<<endl;
    }
}