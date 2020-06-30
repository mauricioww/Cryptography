// -Djava.library.path=.

public class javaDriver{  

   static {
        System.loadLibrary("middleware");
    }
   
    private native String Compute(String source, String destiny);
 
    public static void main(String[] args) {
        if(args.length != 2){
            System.out.println("Bad usage! sourceFile.ext destinyFile ext");
            System.exit(1);
        }
        String result = new javaDriver().Compute(args[0], args[1]);
        System.out.println("Message from C++: " + result);
    }
}