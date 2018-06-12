import java.io.*;
import java.net.*;

class BadClient {

    public static void main(String argv[]) throws Exception {

        String messageFromServer;
        Socket clientSocket = new Socket("localhost", 8686);
        BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
        System.out.println(inFromServer.readLine());
        outToServer.writeBytes("LOGIN s426191\n");
        System.out.println("LOGIN s426191");
        System.out.println(inFromServer.readLine());
        System.out.println(inFromServer.readLine());
        System.out.println(inFromServer.readLine());
        while (true) {
            outToServer.writeBytes("BEGIN N");
            System.out.println("I am blocking the server!!!");
        }
        /*outToServer.writeBytes("BEGIN N\n");
        System.out.println("BEGIN N");
        messageFromServer = inFromServer.readLine();
        System.out.println(messageFromServer);
        System.out.println(inFromServer.readLine());
        while (messageFromServer.equals("OK")) {
            System.out.println(inFromServer.readLine());
            for (int j = 0; j < 50; j++) {
                System.out.println(inFromServer.readLine());
            }
            outToServer.writeBytes("MOVE S\n");
            System.out.println("MOVE S");
            messageFromServer = inFromServer.readLine();
            System.out.println(messageFromServer);
        }
        for (int i = 0; i < 4; i++) {
            System.out.println(inFromServer.readLine());
            System.out.println(inFromServer.readLine());
            outToServer.writeBytes("BEGIN N\n");
            System.out.println("BEGIN N");
            messageFromServer = inFromServer.readLine();
            System.out.println(messageFromServer);
            System.out.println(inFromServer.readLine());
            while (messageFromServer.equals("OK")) {
                System.out.println(inFromServer.readLine());
                for (int j = 0; j < 50; j++) {
                    System.out.println(inFromServer.readLine());
                }
                outToServer.writeBytes("MOVE S\n");
                System.out.println("MOVE S");
                messageFromServer = inFromServer.readLine();
                System.out.println(messageFromServer);

            }

        }
        System.out.println(inFromServer.readLine());
        clientSocket.close();*/
    }
}
