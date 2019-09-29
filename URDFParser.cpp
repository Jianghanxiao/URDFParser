#include <iostream>
#include <vector>
#include <map>

typedef struct link
{
    std::string link_name = "";
    std::string mesh_name = "";
    struct link *parent_link = NULL;
    std::vector<struct link *> child_link;
} Link;

typedef struct joint
{
    std::string parent_name = "";
    std::string child_name = "";
} Joint;

int main()
{
    // Locate the file
    std::string file_name = "../106.URDF";
    if (freopen(file_name.c_str(), "r", stdin) == NULL)
    {
        std::cout << "File doesn't exist\n";
        return 0;
    }
    freopen("../ParseResult.txt", "w", stdout);

    // Parse the URDF
    std::string line;
    int index = -1;
    int name_index1 = -1;
    int name_index2 = -1;
    bool is_link = false, is_joint = false;
    std::vector<Link *> link_vec;
    std::vector<Joint *> joint_vec;
    std::map<std::string, Link *> name_link_map;

    while (std::getline(std::cin, line))
    {
        if (line != "")
        {
            //std::cout << line << "\n";
            // Judge whether this line is used to define the link
            index = line.find("<link");
            if (index != -1)
            {
                is_link = true;
                name_index1 = line.find("\"");
                name_index2 = line.find("\"", name_index1 + 1);
                link_vec.push_back(new Link);
                link_vec[link_vec.size() - 1]->link_name = line.substr(name_index1 + 1, name_index2 - name_index1 - 1);
                name_link_map[link_vec[link_vec.size() - 1]->link_name] = link_vec[link_vec.size() - 1];
            }
            // Get the mesh filename
            index = line.find("<mesh");
            if (index != -1 && is_link == true)
            {
                name_index1 = line.find("\"");
                name_index2 = line.find("\"", name_index1 + 1);
                link_vec[link_vec.size() - 1]->mesh_name = line.substr(name_index1 + 1, name_index2 - name_index1 - 1);
                //std::cout << name_link_map[link_vec[link_vec.size() - 1]->link_name] << "; " << link_vec[link_vec.size() - 1] << "\n";
            }

            index = line.find("</link");
            if (index != -1)
            {
                is_link = false;
            }

            // Judge whether this line is to define a joint
            index = line.find("<joint");
            if (index != -1)
            {
                is_joint = true;
                joint_vec.push_back(new Joint);
            }
            // Get the parent name
            index = line.find("<parent");
            if (index != -1 && is_joint == true)
            {
                name_index1 = line.find("\"");
                name_index2 = line.find("\"", name_index1 + 1);
                joint_vec[joint_vec.size() - 1]->parent_name = line.substr(name_index1 + 1, name_index2 - name_index1 - 1);
            }
            // Get the child name
            index = line.find("<child");
            if (index != -1 && is_joint == true)
            {
                name_index1 = line.find("\"");
                name_index2 = line.find("\"", name_index1 + 1);
                joint_vec[joint_vec.size() - 1]->child_name = line.substr(name_index1 + 1, name_index2 - name_index1 - 1);
            }

            index = line.find("</joint");
            if (index != -1)
            {
                is_joint = false;
            }
        }
    }
    if (is_link == true || is_joint == true)
    {
        std::cout << "Parse Failed\n";
        return 0;
    }

    // Test link and joint
    // for (int i = 0; i <= link_vec.size() - 1; ++i)
    // {
    //     std::cout << "Link: " << link_vec[i].link_name << " " << link_vec[i].mesh_name << "\n";
    // }
    // for (int i = 0; i <= joint_vec.size() - 1; ++i)
    // {
    //     std::cout << "Joint: " << joint_vec[i].parent_name << " " << joint_vec[i].child_name << "\n";
    // }

    // Test the map information
    // for (auto n : name_link_map)
    // {
    //     std::cout << n.first << "; " << n.second->link_name << "\n";
    // }

    // Construct the tree from link and joint
    // The root node should be "0:virtual"
    for (int i = 0; i <= joint_vec.size() - 1; ++i)
    {
        std::string parent_name = joint_vec[i]->parent_name;
        std::string children_name = joint_vec[i]->child_name;
        Link *parent = name_link_map[parent_name];
        Link *children = name_link_map[children_name];

        children->parent_link = parent;
        parent->child_link.push_back(children);
        //std::cout <<parent_name << "; " << children_name << "\n";
    }

    for (int i = 0; i <= link_vec.size() - 1; ++i)
    {
        std::cout << link_vec[i]->link_name << "; ";
        if (link_vec[i]->parent_link != NULL)
        {
            std::cout << link_vec[i]->parent_link->link_name;
        }
        std::cout << "\n";
    }

    for (int i = 0; i <= link_vec.size() - 1; ++i)
    {
        std::cout << link_vec[i]->link_name;
        std::cout << " -- " << link_vec[i]->child_link.size() << ": ";
        for (int j = 0; j <= int(link_vec[i]->child_link.size()) - 1; ++j)
        {
          std::cout << link_vec[i]->child_link[j]->link_name << " ";
        }
        std::cout << "!!\n";
    }


    fclose(stdin);
    fclose(stdout);
    return 0;
}