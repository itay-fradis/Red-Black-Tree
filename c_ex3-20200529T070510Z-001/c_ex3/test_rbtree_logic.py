from ctypes import *
import random
import logging
import subprocess
import os
import uuid
import pytest
import urllib
import pydot
import webbrowser

SHOW_PLOTS_ON_ERROR = True
PRINT_PLOT_URL = False
INPUT_LEN = 15
RANDOM_INPUT_ITERATIONS = 10
RANDOM_DELETE_ITERATIONS = 10

SO_NAME = "./RBTreeTest.so"
RBTREE_FILE = "RBTree.c"
UTIL_FUNCS = "testUtils.c"


RED, BLACK = 0, 1


class Vector(Structure):   
    _fields_ = [("len",c_int),
                ("vector",POINTER(c_double))]

class Node(Structure):
    pass

Node._fields_ = [("parent",POINTER(Node)),
                ("left",POINTER(Node)),
                ("right", POINTER(Node)),
                ("color", c_int),
                ("data",c_void_p)]

class RBTree(Structure):
    _fields_ = [("root",POINTER(Node)),
                ("compFunc", CFUNCTYPE(c_int, c_void_p, c_void_p)),
                ("freeFunc",CFUNCTYPE(None, c_void_p)),
                ("size",c_ulong)]

    def printInOrder(self):
        self.printInOrderRecurse(self.root)

    def printInOrderRecurse(self, node_pointer):
        if node_pointer:
            self.printInOrderRecurse(node_pointer.contents.left)
            print(node_pointer.contents.data.contents.value, "BLACK" if node_pointer.contents.color else "RED")
            self.printInOrderRecurse(node_pointer.contents.right)

    def blackPathLen(self):
        node = self.root
        pathlen = -1
        while node:
            if node.contents.color == BLACK:
                pathlen += 1
            node = node.contents.left
        return pathlen

    def isvalidTree(self, compFunc=None):
        if not self.root:
            return (True, "Tree is empty")
        if self.root.contents.color != BLACK:           
            return (False, "root is not black")
        blackpath_len = self.blackPathLen()
        return self.isvalidTreeRecurse(self.root, blackpath_len, -1)

    def isvalidTreeRecurse(self, node, blackpath_len, pathtonode_len):
        if not node:
            if blackpath_len != pathtonode_len:
                print(pathtonode_len, blackpath_len)
                return (False, "Black path length is not equal on all leaves")
            return (True, "Tree is valid")
        if (node.contents.parent and node.contents.color == RED and 
           (node.contents.parent.contents.color == node.contents.color)):
            return (False, "Two consecutive RED nodes")
        if (
            (node.contents.left and 
                0 < self.compFunc(node.contents.left.contents.data, node.contents.data))
           or 
           (node.contents.right and 
               0 < self.compFunc(node.contents.data, node.contents.right.contents.data))
               ):
            return (False, "Tree is not BST")
        if node.contents.color == BLACK:
            pathtonode_len += 1
        left = self.isvalidTreeRecurse(node.contents.left, blackpath_len, pathtonode_len)
        if left[0] is False:
            return left
        right = self.isvalidTreeRecurse(node.contents.right, blackpath_len, pathtonode_len)
        if right[0] is False:
            return right
        
        return (True, "Tree is valid")

    def plot(self, printFunc=None):
        graph = pydot.Dot(graph_type='graph')
        self.tree_to_graph(self.root, graph)
        url = "https://dreampuf.github.io/GraphvizOnline/#" + urllib.parse.quote(graph.to_string())
        webbrowser.open_new(url)
        return url

    def tree_to_graph(self, root, graph):
        if not root:
            label = ''
            leaf = pydot.Node(str(uuid.uuid4()), label=label, shape="box", style='filled', fillcolor="black",  width=.1, height=.1)
            graph.add_node(leaf)
            return leaf

        label = cast(root.contents.data, POINTER(c_int)).contents.value
        color = 'red' if (root.contents.color == RED) else 'black'
        g_node = pydot.Node(id(root), label=label, style='filled', fontcolor='white', shape="circle", fillcolor=color)
        graph.add_node(g_node)

        neg_node = self.tree_to_graph(root.contents.left, graph)
        graph.add_edge(pydot.Edge(g_node, neg_node))
        pos_node = self.tree_to_graph(root.contents.right, graph)
        graph.add_edge(pydot.Edge(g_node, pos_node))

        return g_node



def test_insert():
    cfile = CDLL(SO_NAME)
    cfile.newRBTree.restype = POINTER(RBTree)
    inp = list([c_int(i) for i in range(INPUT_LEN)])
    for i in range(RANDOM_INPUT_ITERATIONS):
        random.shuffle(inp) #shuffle input
        tree = cfile.newRBTree(cfile.intCompFunc, cfile.freeFunc)
        logging.debug(f"Created new RBTree")
        logging.debug(f"Input: {[n.value for n in inp]}")
        
        for index, i in enumerate(inp):
            logging.debug(f"Inserting {i.value}")
            cfile.insertToRBTree(tree, byref(i))
            logging.debug(f"Inserted {i.value} successfully")
            isvalid, msg = tree.contents.isvalidTree()
            if isvalid is False:
                logging.error("RBTree insert failed!")
                logging.error(f"Input: {[n.value for n in inp]}")
                logging.error(f"Inserting {i.value} corrupted the tree.")
                logging.error(f"Tree error is: {msg}.")

                if (SHOW_PLOTS_ON_ERROR):
                    url_after = tree.contents.plot() # plot corrupted
                    tree = cfile.newRBTree(cfile.intCompFunc, cfile.freeFunc)
                    for j in range(index):
                        cfile.insertToRBTree(tree, byref(inp[j]))
                    url_before = tree.contents.plot() # plot before corruption

                    if PRINT_PLOT_URL:
                        logging.error(f"Tree before: {url_before}")
                        logging.error(f"Tree after: {url_after}")

                assert isvalid
            logging.debug(f"Inserting {[n.value for n in inp]} passsed successfully")

def test_delete():
    cfile = CDLL(SO_NAME)
    cfile.newRBTree.restype = POINTER(RBTree)
    tree = cfile.newRBTree(cfile.intCompFunc, cfile.freeFunc)
    logging.debug(f"Created new RBTree")
    inp = list([c_int(i) for i in range(INPUT_LEN)])
    for iter in range(RANDOM_DELETE_ITERATIONS):
        random.shuffle(inp)
        logging.debug(f"Input: {[n.value for n in inp]}")
        for i in inp:
            cfile.insertToRBTree(tree, byref(i))
        isvalid, msg = tree.contents.isvalidTree()
        assert isvalid == True
        logging.debug(f"Inserting {[n.value for n in inp]} passsed successfully")
        deleted = []
        for i in range(INPUT_LEN):
            data_to_del = random.choice(inp)
            logging.debug(f"deleting {data_to_del.value}")
            cfile.deleteFromRBTree(tree, byref(data_to_del))
            isvalid, msg = tree.contents.isvalidTree()
            if isvalid is False:
                logging.error("RBTree delete failed!")
                logging.error(f"Input: {[n.value for n in inp]}")
                logging.error(f"Deleted so far: {[n.value for n in deleted]}")
                logging.error(f"Deleting {data_to_del.value} corrupted the tree.")
                logging.error(f"Tree error is: {msg}.")
                
                if (SHOW_PLOTS_ON_ERROR):
                    url_after = tree.contents.plot() # plot corrupted
                    tree = cfile.newRBTree(cfile.intCompFunc, cfile.freeFunc)
                    for j in inp:
                        cfile.insertToRBTree(tree, byref(j))
                    for d in deleted:
                        cfile.deleteFromRBTree(tree, byref(d))
                    url_before = tree.contents.plot() # plot before corruption
                    if PRINT_PLOT_URL:
                        logging.error(f"Tree before: {url_before}")
                        logging.error(f"Tree after: {url_after}")

                assert isvalid == True
            deleted.append(data_to_del)
        logging.debug(f"deleting {[n.value for n in deleted]} passed succesfully")

@pytest.fixture(scope="session", autouse=True)
def compile(request):
    try:
        subprocess.run(["cc", "-fPIC", "-shared", "-o", SO_NAME, RBTREE_FILE, UTIL_FUNCS], check=True)
    except subprocess.CalledProcessError:
        logging.error(f"Unable to complie {RBTREE_FILE}")
        raise AssertionError(f"Unable to complie {RBTREE_FILE}")
