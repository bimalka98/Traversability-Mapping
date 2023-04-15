## April 10th, 2023 

1. Add a comment on original repo's issue section, where they have solved exactly the same problem. I think it's a good idea to add a link to the issue in the README.md file. They may share the code probably.

        https://github.com/TixiaoShan/traversability_mapping/issues/7#issuecomment-1501733263 
        
2. There's a node named `traversability_path` ,but in your launch ,you dont use this node.Could you tell me about this node meaning?This file is another planner. It's using pre-computed paths. Here is a video of it. Because I stopped working on this project, so this planner has some issues when used. https://youtu.be/Wrs_m1KCT2g

        <node pkg="traversability_mapping" type="traversability_path"    name="traversability_path"    output="screen"/>
    
## April 13th, 2023

* Realizing the PRM graph structure to load and save it efficiently.
* So we can save it at first run and then simply load it in the next run for path planning.

```cpp
// PRM graph structure
vector<state_t*> nodeList;

struct state_t{
    double x[3]; //  1 - x, 2 - y, 3 - z
    float theta;
    int stateId;
    float cost;
    bool validFlag;
    // # Cost types 0. obstacle cost 1. elevation cost 2. distance cost
    float costsToRoot[NUM_COSTS];

    state_t* parentState; // parent for this state in PRM and RRT*
    vector<neighbor_t> neighborList; // PRM adjencency list with edge costs

    // default initialization
    state_t()
    // use a state input to initialize new state    
    state_t(state_t* stateIn)
};

struct neighbor_t{
    state_t* neighbor;
    float edgeCosts[NUM_COSTS]; // the cost from this state to neighbor
    
    // default initialization: do not need to save
    neighbor_t(){
        neighbor = NULL;
        for (int i = 0; i < NUM_COSTS; ++i)
            edgeCosts[i] = FLT_MAX;
    }
};
```
## April 15th, 2023

## BFS Search
* 