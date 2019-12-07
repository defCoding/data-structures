import random

class BTreeNode:
    def __init__(self, degree):
        self.degree = degree
        self.keys = []
        self.min_keys = degree - 1
        self.max_keys = 2 * degree - 1
        self.children = []

    def insert(self, key):
        # Find where the key must be inserted.
        insertion_index = len(self.keys)
        for i in range(0, len(self.keys)):
            if self.keys[i] > key:
                insertion_index = i
                break

        # Assumes that if it is a leaf, it does not have max_keys already.
        if self.is_leaf():
            self.insert_to_keys(key, insertion_index)

        else:
            child_to_insert = self.children[insertion_index]

            if len(child_to_insert.keys) < self.max_keys:
                child_to_insert.insert(key)
            else:
                child_median_index = len(child_to_insert.keys) // 2
                
                # Insert median value in child into this node.
                self.insert_to_keys(child_to_insert.keys[child_median_index], insertion_index)

                # Create the nodes that will be to the left and right of the median that was just moved up.
                lNode_of_median = BTreeNode(self.degree)
                rNode_of_median = BTreeNode(self.degree)

                # Fill in the keys of the left and right node.
                for i in range(0, child_median_index):
                    lNode_of_median.keys.append(child_to_insert.keys[i])
                    rNode_of_median.keys.append(child_to_insert.keys[i + child_median_index + 1])

                # Fill in the children of the left and right node.
                if not child_to_insert.is_leaf():
                    for i in range(0, child_median_index + 1):
                        lNode_of_median.children.append(child_to_insert.children[i])
                        rNode_of_median.children.append(child_to_insert.children[i + child_median_index + 1])

                # Insert the newly filled children into the list of childrens.
                self.children[insertion_index] = lNode_of_median
                self.insert_to_children(rNode_of_median, insertion_index + 1)

                # Insert key into correct child.
                if key < self.keys[insertion_index]:
                    lNode_of_median.insert(key)
                else:
                    rNode_of_median.insert(key)
            


    def insert_to_keys(self, key, index):
        for i in range(index, len(self.keys)):
            tmp = self.keys[i]
            self.keys[i] = key
            key = tmp
        
        self.keys.append(key)

    def insert_to_children(self, child, index):
        for i in range(index, len(self.children)):
            tmp = self.children[i]
            self.children[i] = child
            child = tmp
        
        self.children.append(child)
    
    def inorder_traversal(self):
        print("[", end = " ")
        for i in range(0, len(self.keys)):
            if not self.is_leaf():
                self.children[i].inorder_traversal()
            print(self.keys[i], end = " ")

        if not self.is_leaf(): 
            self.children[len(self.keys)].inorder_traversal()
        print("]", end = " ")

    def is_leaf(self):
        return len(self.children) == 0

    def is_full(self):
        return len(self.keys) == self.max_keys


class BTree:
    def __init__(self, degree):
        self.root = None
        self.degree = degree
    
    def insert(self, key):
        if self.root is None:
            self.root = BTreeNode(self.degree)
            self.root.keys.append(key)
        else:
            if self.root.is_full():
                median_index = len(self.root.keys) // 2

                # Create new root and its two children.
                new_root = BTreeNode(self.root.degree)
                left_node = BTreeNode(self.root.degree)
                right_node = BTreeNode(self.root.degree)

                new_root.keys.append(self.root.keys[median_index])

                # Fill in keys of children.
                for i in range(0, median_index):
                    left_node.keys.append(self.root.keys[i])
                    right_node.keys.append(self.root.keys[i + median_index + 1])

                # Fill in children of children only if the original root was not a leaf.
                if not self.root.is_leaf():
                    for i in range(0, median_index + 1):
                        left_node.children.append(self.root.children[i])
                        right_node.children.append(self.root.children[i + median_index + 1])
                
                new_root.children.append(left_node)
                new_root.children.append(right_node)

                # Insert key to the left or right depending on relation to the key in the new_root.
                if key < new_root.keys[0]:
                    left_node.insert(key)
                else:
                    right_node.insert(key)

                # Set the new root.
                self.root = new_root
            else:
                self.root.insert(key)

    def inorder_traversal(self):
        self.root.inorder_traversal()
        print()

def run():
    bTree = BTree(4)

    for i in range(0, 100):
        bTree.insert(random.randint(0, 500))

    bTree.inorder_traversal()
    
if __name__ == "__main__":
    run()