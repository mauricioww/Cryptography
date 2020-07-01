// -Djava.library.path=.

public class javaDriver{  

   static {
        System.loadLibrary("middleware");
    }
   
    private native String Compute(String source, String destiny, String ext);
 
    public static void main(String[] args) {
        if(args.length != 3){
            System.out.println("Bad usage! arg0 = 'sourceFile.ext', arg1 = 'destinyFile', arg2 = 'ext'");
            System.exit(1);
        }
        String result = new javaDriver().Compute(args[0], args[1], args[2]);
        System.out.println("Message from C++: " + result);
    }
}