package Code.Stack;
import java.util.logging.Logger;


public class LinkedStack
{

    private Node topNode;
    private Node bottomNode;
    private Logger log = Logger.getGlobal();//log
    private int len;

    LinkedStack()
    {
        bottomNode= new Node();
        topNode=bottomNode;
        len=0;
    }


    //压栈
    public boolean push(Object elem)
    {
        
        Node node = new Node(elem,null);

        if(len!=0)
        {
            topNode.nextNode=node;
        }
        else
        {
            bottomNode.nextNode=node;
        }

        topNode = node;
        len++;
        return true;

    }


    //获得栈顶元素
    public Object getTop()
    {
        log.info(topNode.data.toString()+":"
        +topNode.data.getClass().toString());
        return topNode.data;
    }


    //删
    public boolean pop()
    {
        Node preNode = bottomNode;
        len--;

        for(int i= 0;i<len;i++)
        {
            preNode=preNode.nextNode;
        }

        preNode.nextNode=null;

        topNode=preNode;

        return true;
    }

    //是否空栈
    public boolean isEmpty()
    {
        if(len==0)
        {
            log.info("true");
            return true;
        }
        else
        {
            log.info("false");
            return false;
        }
    }

    //全栈删除
    public boolean deleteAll()
    {
        bottomNode.nextNode=null;
        len=0;
        topNode=bottomNode;
        return true;
    }


    //返回个数
    public int length()
    {
        return len;
    }

}



class test
{
    public static void main(String[] args)
    {
        LinkedStack stack  = new LinkedStack();
        stack.push(12);
        stack.getTop();
        stack.push("aa");
        stack.getTop();
        stack.push(12.00);
        stack.getTop();
        stack.push(true);
        stack.getTop();
        stack.pop();
        stack.pop();
        stack.pop();
        stack.pop();
        stack.isEmpty();
        stack.push(false);
        stack.getTop();
    }
}
