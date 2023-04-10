## April 10th, 2023 

1. Add a comment on original repo's issue section, where they have solved exactly the same problem. I think it's a good idea to add a link to the issue in the README.md file. They may share the code probably.

        https://github.com/TixiaoShan/traversability_mapping/issues/7#issuecomment-1501733263 
        
2. There's a node named `traversability_path` ,but in your launch ,you dont use this node.Could you tell me about this node meaning?This file is another planner. It's using pre-computed paths. Here is a video of it. Because I stopped working on this project, so this planner has some issues when used. https://youtu.be/Wrs_m1KCT2g

        <node pkg="traversability_mapping" type="traversability_path"    name="traversability_path"    output="screen"/>
    