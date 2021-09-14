package Code.Stack;

public class Node
{
    Object data;
    Node nextNode;
    public Node(Object data,Node nextNode)
    {
        this.data=data;
        this.nextNode=nextNode;
    }

    public Node()
    {
        this(null, null);
    }
}
