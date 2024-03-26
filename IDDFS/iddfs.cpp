/*
 This is work of Raunak Kumar Singh as a part of CS411 Artificial Intelligence coursework.
*/
#include<iostream>
#include <vector>
#include <stack>

using namespace std;

int expanded_nodes = 0; //variable that will hold no of expanded nodes
vector<vector<int>> goal = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}}; //Goal state 2D matrix
char direction[] = { 'U', 'D', 'L', 'R'};
int row_direction[] = {-1,1,0,0}; //row configuration for moving through matrix
int col_direction[] = {0,0,-1,1}; //col configuration for moving through matrix
int memory = 0;

struct coordinates{
    int x;
    int y;
};

//Node
struct node{
    vector<vector<int> > state;
    coordinates blank_coordinate;
    vector<char> path;
    int depth;
    struct node* parent;
};

//function to check if the given configuration is solvable
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

//function to check if the node has been explored before
bool is_cycle(node* child){
    node* temp  = child;
    while(child!= NULL){
        child = child->parent;
        if(child == temp){
            return true;
        }
    }
    return false;
}

//Function to expand nodes
vector<node*> expand_nodes(node* current){
    expanded_nodes++;
    vector<node*> child_nodes; //A list of child nodes to be returned
    int x = current->blank_coordinate.x;
    int y = current->blank_coordinate.y;
    for(int i=0;i<4;i++){
        struct node* new_node = (struct node*)malloc(sizeof(struct node));
        new_node->state = copy_state(current->state);
        new_node->path = current->path;
        new_node->path.push_back(direction[i]);
        new_node->depth = current->depth +1; //The child of parent is at a level deeper
        new_node->parent = current; //assigning parent to the child node
        int new_x = x+row_direction[i];
        int new_y = y+col_direction[i];
        if(new_x >=0 && new_x<4 && new_y>=0 && new_y<4){ //checking if the move is possible
            swap(new_node->state[new_x][new_y], new_node->state[x][y]);
            new_node->blank_coordinate.x = new_x;
            new_node->blank_coordinate.y = new_y;
            child_nodes.push_back(new_node);
        }
    }
    return child_nodes;
};

//Implementation of Depth limited search
int depth_limited_search(node* initial, int l){
    stack<node*> frontier;
    frontier.push(initial);
    while(!frontier.empty()){
        node* temp = frontier.top();
        frontier.pop();
        if(check_goal(temp)){
            cout<<"Moves: ";
            for(int i=0;i<temp->path.size();i++){
                cout<<temp->path[i];
            }
            cout<<endl;
            memory = sizeof(frontier)*frontier.size();
            return 1;
        }
        if(temp->depth < l){ //exploring the nodes at depth less than l
            for(auto child: expand_nodes(temp)){
                if(!is_cycle(child)){
                    child->depth = temp->depth + 1;
                    frontier.push(child);
                }
            }
        }
    }
    return 0;
}

//Implementation of IDS
int iterative_deepening_search(node* initial){
    for(int i =0;;i++){
        int result = depth_limited_search(initial,i);
        if(result){
            return 1;
        }
    }
    return 0;
}


int main(){
    vector<vector<int> > initial= { {1,0,2,4},{5,7,3,8},{9,6,11,12},{13,10,14,15} };
    // vector<vector<int> > initial;
    // for(int i=0;i<4;i++){
    //     vector<int> temp;
    //     for (int j = 0; j < 4; j++)
    //     {
    //        int x;
    //        cin>>x;
    //        temp.push_back(x);
    //     }
    //     initial.push_back(temp);
    // }
    node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->state = copy_state(initial);
    new_node->depth = 0; //Assigning the depth of 0 to initial state
    new_node->parent = NULL; //Making the parent of initial state as null
    for(int i =0;i<4;i++){
        for(int j=0;j<4;j++){
            if(initial[i][j]==0){
                new_node->blank_coordinate.x = i;
                new_node->blank_coordinate.y = j;
            }
        }
    }

    if(is_solvable(initial)){
        auto start_time = std::chrono::high_resolution_clock::now(); //starting point of time
        int result = iterative_deepening_search(new_node);
        if(result == 1){
            auto end_time = std::chrono::high_resolution_clock::now(); //ending point of time
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);//total time taken for bfs to run
            cout<<"Number of Nodes expanded: "<< expanded_nodes <<endl;
            cout<<"Time Taken (in miliseconds): "<< duration.count()/1000<<endl;
            cout<<"Memory Used: "<<memory << " bytes" << endl;
        }
    }
    else
        cout<<"Solution doesn't exist for the given configuration"<<endl;
    return 0;
}