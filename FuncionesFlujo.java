package Controlador;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class FuncionesFlujo {
    ServerSocket s;
    
    public static final String RUTA = "Carpetas/";
    
    private static FuncionesFlujo INSTANCE = null;
    private static final int TAM = 1024;
    
    /*
     * Constructor de este servidor
     */
    private FuncionesFlujo(int puerto) throws IOException {
        s = new ServerSocket(puerto);
    }
    
    /*
     * Constructor de instancia de este servidor
     */
    public static FuncionesFlujo getInstance(int puerto) throws IOException{
        if(INSTANCE == null){
            INSTANCE = new FuncionesFlujo(puerto);
        }
        
        return INSTANCE;    
    }
    
    /*
    * Método para enviar un archivo a un nodo que lo solicite
    */
    public boolean enviarArchivo(String carpeta, Funciones tl) throws IOException {
        //Estoy esperando una pinche puta conexión
        Socket cl = s.accept();
        
        if(tl != null){
            tl.mensaje("Conexión establecida desde" + cl.getInetAddress() + ":"+ cl.getPort());  
        }    

        DataInputStream disa = new DataInputStream(cl.getInputStream());

        byte b[] = new byte[TAM];
        String nombre = disa.readUTF();
        
        if(tl != null){
            tl.mensaje("Transferir " + nombre);
        }
        
        //Establezco la puta ruta
        String path = RUTA + "/" + carpeta  + "/" + nombre;
        File f = new File(path);
        
        DataInputStream dis = new DataInputStream(new FileInputStream(path));
        DataOutputStream dos = new DataOutputStream(cl.getOutputStream());
        
        long enviados = 0;
        int porcentaje, n;
        long tam = f.length();
        
        dos.writeLong(tam);
        
        //Envio los putos bytes del archivo y checo su puto porcentaje
        while ((n = dis.read(b)) > 0) {       
            dos.write(b, 0, n);
            dos.flush();
            
            if(tl != null){
                enviados = enviados + n;
                porcentaje = (int) (enviados * 100 / tam);
                tl.porcentaje(porcentaje);
            }
        }
        
        //A la verga los OutputStreams
        dos.close();
        dis.close();
        disa.close();
        cl.close();
        
        //Ya rolé el archivo
        if(tl != null){
            tl.mensaje("Archivo transferido");
        }
        
        return true;
    }
    
    public boolean solicitarArchivo(String ip, int puerto, String nombreArchivo, String carpeta, Funciones tl) throws IOException{
        //Creo una conexión
        Socket cl = new Socket(ip, puerto);
        
        //Establezco la ruta donde quiere que me lo rolen
        String path = RUTA + "/" + carpeta+ "/" + nombreArchivo;
        
        DataOutputStream dos = new DataOutputStream(cl.getOutputStream());
        DataOutputStream dosa = new DataOutputStream(new FileOutputStream(path));
        DataInputStream dis = new DataInputStream(cl.getInputStream());
        
        dos.writeUTF(nombreArchivo);

        Long tam = dis.readLong();
        
        long recibidos = 0;
        int n, porcentaje;
        byte b[] = new byte[TAM];
        
        // A partir de aquí voy a recibir el archivo
        if(tl!= null){
            tl.mensaje("Recibiendo archivo...");
        }
        
        //Recibo sus bytes y el porcentaje de completitud
        while ((n = dis.read(b)) > 0) {
            dosa.write(b, 0, n);
            dosa.flush();
            
            if(tl != null){ 
                recibidos = recibidos + n;
                porcentaje = (int)(recibidos * 100 / tam);
                tl.porcentaje(porcentaje);
            }
        }
        
        // Ya me lo rolaron
        tl.mensaje("Archivo recibido");
        
        // Adiós OutputStreams
        dos.close();
        dosa.close();
        dis.close();
        cl.close();
        
        return true;
    }
}