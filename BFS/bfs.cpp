/*
 This belongs to Raunak Kumar Singh
*/
#include<iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>

using namespace std;

int expanded_nodes = 0; //calculate the no of nodes expanded
int memory = 0;
vector<vector<int>> goal = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}}; //Goal state 2D matrix
set< vector<vector<int> > > reached; //Hashset to keep track of visited states
char direction[] = { 'U', 'D', 'L', 'R'};
int row_direction[] = {-1,1,0,0}; //row configuration for moving through matrix
int col_direction[] = {0,0,-1,1}; //col configuration for moving through matrix

struct coordinates{
    int x;
    int y;
};

//Node 
struct node{
    vector<vector<int> > state;
    coordinates blank_coordinates;
    vector<char> path;
};

//function to copy the state while creating a new node
vector<vector<int> > copy_state(vector<vector<int> > s){
    vector<vector<int> > state;
    for(int i=0;i<s.size();i++){
        vector<int> temp;
        for(int j=0;j<s.size();j++){
            temp.push_back(s[i][j]);
        }
        state.push_back(temp);
    }
    return state;
};

//function to check if the state is goal state
bool check_goal(node* curr){
    vector<vector<int>> a = curr->state;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(a[i][j] != goal[i][j])
                return false;
        }
    }
    return true;
};

//function to check if the state has previously been reached
bool is_reached(node* curr){
    vector<vector<int> > a = curr->state;
    for (set<vector<vector<int> >>::iterator itr = reached.begin(); itr != reached.end(); itr++) {
        if(a == *itr){
            return true;
        }
    }
    return false;
};

bool is_solvable(vector<vector<int> > initial){
    vector<int> arr;
    int blankrow;
    int parity =0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(initial[i][j]==0)
                blankrow=i+1;
            arr.push_back(initial[i][j]);
        }
    }

    for(int i=0;i<arr.size();i++){
        for(int j=i+1;j<arr.size();j++){
            if(arr[i]>arr[j] && arr[j] != 0){
                parity++;
            }
        }
    }

    if (blankrow % 2 == 0) { // blank on odd row; counting from bottom
            return parity % 2 == 0;
        } else { // blank on even row; counting from bottom
            return parity % 2 != 0;
    }
}

//function to expand the nodes 
vector<node*> expand_nodes(node* current){
    expanded_nodes++;
    vector<node*> child_nodes;
    int x = current->blank_coordinates.x;
    int y = current->blank_coordinates.y;
    for(int i=0;i<4;i++){
        struct node* new_node = (struct node*)malloc(sizeof(struct node)); //creating a new node
        new_node->state = copy_state(current->state); 
        new_node->path = current->path; 
        new_node->path.push_back(direction[i]);
        int new_x = x+ row_direction[i];
        int new_y = y+ col_direction[i];
        if(new_x >=0 && new_x<4 && new_y>=0 && new_y<4){ 
            swap(new_node->state[new_x][new_y],new_node->state[x][y]); //swapping the states between old and new state
            new_node->blank_coordinates.x=new_x;
            new_node->blank_coordinates.y=new_y;
            child_nodes.push_back(new_node);
        }
    }
    return child_nodes;
};

//bfs
int bfs(node* start){
    queue<node*> frontier;
    frontier.push(start);
    while(!frontier.empty() ){
        node* x = frontier.front();
        frontier.pop();
        reached.insert(x->state);
        if(goal == x->state){ //checking if the current state is the goal state
            cout<<"Moves: ";
            for(int i=0;i<x->path.size();i++){
                cout<<x->path[i];
            }
            cout<<endl;
            memory = sizeof(frontier); //calculating the memory occupied by frontier
            return 1;
        }
        for(node* child: expand_nodes(x)) {
            //checking for repeated states 
            if(!is_reached(child) ){ 
                reached.insert(child->state);
                frontier.push(child);
            }
        }
    }
    return 0;
}

int main() {
    // vector<vector<int> > initial= { {1,0,2,4},{5,7,3,8},{9,6,11,12},{13,10,14,15} };
    vector<vector<int> > initial;
    for(int i=0;i<4;i++){
        vector<int> temp;
        for (int j = 0; j < 4; j++)
        {
           int x;
           cin>>x;
           temp.push_back(x);
        }
        initial.push_back(temp);
    }

    node* s = (struct node*)malloc(sizeof(struct node));
    s->state = copy_state(initial);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(initial[i][j]==0){
                s->blank_coordinates.x=i;
                s->blank_coordinates.y=j;
            }
        }
    }

    if(is_solvable(initial)){
        auto start_time = std::chrono::high_resolution_clock::now(); //starting point of time
        int result = bfs(s);
        if(result == 1){
            auto end_time = std::chrono::high_resolution_clock::now(); //ending point of time
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);//total time taken for bfs to run
            memory+= (reached.size()*sizeof(goal)); //total memory taken by the program
            cout<<"Number of Nodes expanded: "<< expanded_nodes <<endl;
            cout<<"Time Taken (in miliseconds): "<< duration.count()/1000<<endl;
            cout<<"Memory Used: "<<memory/1024 << " Kb" << endl;
        }
    }
    else
        cout<<"Solution doesn't exist for the given configuration"<<endl;
    return 0;
}
