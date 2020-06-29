public class HelloJNI{  

   static {
        System.loadLibrary("hello"); // Load native library hello.dll (Windows) or libhello.so (Unixes) at runtime
    }                              // This library contains a native method called sayHello()
   

    // Declare an instance native method sayHello() which receives no parameter and returns void
    private native String sayHello(String source, String detiny);
 
    // Test Driver
    public static void main(String[] args) {
        // String result = new HelloJNI().sayHello("Hello from java!");  // Create an instance and invoke the native method
        String result = new HelloJNI().sayHello(args[0], args[1]);  // Create an instance and invoke the native method
        System.out.println("Message from C: " + result);
    }
}