## April 10th, 2023 

1. Add a comment on original repo's issue section, where they have solved exactly the same problem. I think it's a good idea to add a link to the issue in the README.md file. They may share the code probably.

        https://github.com/TixiaoShan/traversability_mapping/issues/7#issuecomment-1501733263 
        
2. There's a node named `traversability_path` ,but in your launch ,you dont use this node.Could you tell me about this node meaning?This file is another planner. It's using pre-computed paths. Here is a video of it. Because I stopped working on this project, so this planner has some issues when used. https://youtu.be/Wrs_m1KCT2g

        <node pkg="traversability_mapping" type="traversability_path"    name="traversability_path"    output="screen"/>
    
## April 13th, 2023

* Realizing the PRM graph structure to load and save it efficiently.
* So we can save it at first run and then simply load it in the next run for path planning.

```cpp
// PRM graph structure is a vector of pointers to states
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
};

struct neighbor_t{
    state_t* neighbor;
    float edgeCosts[NUM_COSTS]; // the cost from this state to neighbor
};
```
## April 16th, 2023

* Completed the investigation of dependencies of PRM path planning algorithms.
* Path planning algorithm depends on the following variables:
    - `vector<state_t*> nodeList;` 
    - `kdtree_t *kdtree;`
* Saving and loading the PRM graph structure is the problem at hand.
* `kdtree_t *kdtree;` is impossible to save and load. Found a way to restore it using the data of loaded `vector<state_t*> nodeList;`. But this is yet to be verified.

### Solution 1: Initialized on April 17th, 2023

1. Save the `vector<state_t*> nodeList;` to a file during the program destruction (possibly).
2. Load the `vector<state_t*> nodeList;` from the file during the program initialization (possibly once inside the `buildRoadMap()` fucntion).
```cpp
void buildRoadMap(){
        // 1. generate samples
        generateSamples();
        // 2. Add edges and update state height if map is changed
        updateStatesAndEdges();   
        // 3. Planning
        bfsSearch();
        // 4. Visualize Roadmap
        publishPRM();
        // 5. Publish path to move_base and stop planning
        publishPathStop();
        // 6. Convert PRM Graph into point cloud for external usage
        publishRoadmap2Cloud();
    }
```
3. Restore the `kdtree_t *kdtree;` using the data of loaded `vector<state_t*> nodeList;` and function `insertIntoKdtree(newState)` in the `void generateSamples()` fucntion.
4. Enter to the usual flow of the program.