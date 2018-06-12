import java.io.*;
import java.net.*;
import java.util.*;


class Client extends Thread {
    protected Socket socket;
    protected int id;
    protected String clientMessage;

    public Client(Socket clientSocket, int id) {
        this.socket = clientSocket;
        this.id = id;
    }


    public void round() {

        try {
            boolean terminated = false;
            while (!terminated && Server.playersInactive[id] != 1) {
                Server.semaphore.acquire();
                terminated = Server.play(this, id);
                Server.semaphore.release();
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        finally {
            Server.semaphore.release();
        }

    }

    public void run() {

        try {
            Server.gate.await();
            Server.os[id].writeBytes("GAME\n");
            System.out.println("GAME");

            round();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

    }
}


public class Server
{
    public static int[][] board = new int[50][50];

    public static Socket[] players = new Socket[5];
    public static BufferedReader[] is = new BufferedReader[5];
    public static DataOutputStream[] os = new DataOutputStream[5];
    public static String[] logins = new String[5];
    public static String[] currentDirections = new String[5];

    public static int[] currentX = new int[5];
    public static int[] currentY = new int[5];

    public static int[][] positions = new int[5][5];
    public static int positionIdx = 4;

    public static int round = 1;

    public static Client[] threads = new Client[5];
    public static int[] playersInactive = new int[5];

    public static java.util.concurrent.Semaphore semaphore = new java.util.concurrent.Semaphore(1);
    public static final java.util.concurrent.CyclicBarrier gate = new java.util.concurrent.CyclicBarrier(5);

    public static synchronized boolean play(Client client, int id) {
        try {
            Server.os[id].writeBytes(String.format("BOARD\n"));
            System.out.println("BOARD");
            for (int j = 0; j < 50; j++) {
                for (int k = 0; k < 50; k++) {
                    Server.os[id].writeBytes(String.format("%s ", Server.board[j][k]));
                    System.out.print(String.format("%s ", Server.board[j][k]));
                }
                Server.os[id].writeBytes(String.format("\n"));
                System.out.println();
            }
            client.clientMessage = Server.is[id].readLine().split("\\s+")[1];
            System.out.println(client.clientMessage);
            if (client.clientMessage.equals("R")) {
                if ((Server.currentY[id] + 1 > 49) || (Server.board[Server.currentX[id]][Server.currentY[id] + 1] != 0)) {
                    Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                    if (Server.positionIdx == 0) {
                        Server.os[id].writeBytes(String.format("WIN\n"));
                        System.out.println("WIN");
                    } else {
                        Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                        System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                    }
                    Server.positionIdx--;
                    for (int j = 0; j < 50; j++) {
                        for (int k = 0; k < 50; k++) {
                            System.out.print(String.format("%s ", Server.board[j][k]));
                        }
                        System.out.println();
                    }
                    return true;
                }
                os[id].writeBytes("OK\n");
                System.out.println("OK");
                Server.board[Server.currentX[id]][Server.currentY[id] + 1] = id + 1;
                Server.currentY[id] = Server.currentY[id] + 1;
            } else if (client.clientMessage.equals("L")) {
                if ((Server.currentY[id] - 1 < 0) || (Server.board[Server.currentX[id]][Server.currentY[id] - 1] != 0)) {
                    Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                    if (Server.positionIdx == 0) {
                        Server.os[id].writeBytes(String.format("WIN\n"));
                        System.out.println("WIN");
                    } else {
                        Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                        System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                    }
                    Server.positionIdx--;
                    for (int j = 0; j < 50; j++) {
                        for (int k = 0; k < 50; k++) {
                            System.out.print(String.format("%s ", Server.board[j][k]));
                        }
                        System.out.println();
                    }
                    return true;
                }
                os[id].writeBytes("OK\n");
                System.out.println("OK");
                Server.board[Server.currentX[id]][Server.currentY[id] - 1] = id + 1;
                Server.currentY[id] = Server.currentY[id] - 1;
            } else {
                if (Server.currentDirections[id].equals("N")) {
                    if ((Server.currentX[id] - 1 < 0) || (Server.board[Server.currentX[id] - 1][Server.currentY[id]] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[id].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                        }
                        Server.positionIdx--;
                        for (int j = 0; j < 50; j++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[j][k]));
                            }
                            System.out.println();
                        }
                        return true;
                    }
                    os[id].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[id] - 1][Server.currentY[id]] = id + 1;
                    Server.currentX[id] = Server.currentX[id] - 1;
                } else if (Server.currentDirections[id].equals("S")) {
                    if ((Server.currentX[id] + 1 > 49) || (Server.board[Server.currentX[id] + 1][Server.currentY[id]] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[id].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                        }
                        Server.positionIdx--;
                        for (int j = 0; j < 50; j++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[j][k]));
                            }
                            System.out.println();
                        }
                        return true;
                    }
                    os[id].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[id] + 1][Server.currentY[id]] = id + 1;
                    Server.currentX[id] = Server.currentX[id] + 1;
                } else if (Server.currentDirections[id].equals("E")) {
                    if ((Server.currentY[id] + 1 > 49) || (Server.board[Server.currentX[id]][Server.currentY[id] + 1] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[id].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                        }
                        Server.positionIdx--;
                        for (int j = 0; j < 50; j++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[j][k]));
                            }
                            System.out.println();
                        }
                        return true;
                    }
                    os[id].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[id]][Server.currentY[id] + 1] = id + 1;
                    Server.currentY[id] = Server.currentY[id] + 1;
                } else if (Server.currentDirections[id].equals("W")) {
                    if ((Server.currentY[id] - 1 < 0) || (Server.board[Server.currentX[id]][Server.currentY[id] - 1] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = id + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[id].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[id].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                        }
                        Server.positionIdx--;
                        for (int j = 0; j < 50; j++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[j][k]));
                            }
                            System.out.println();
                        }
                        return true;
                    }
                    os[id].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[id]][Server.currentY[id] - 1] = id + 1;
                    Server.currentY[id] = Server.currentY[id] - 1;
                }
            }
            return false;

        }
        catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }

    public static void main(String argv[]) throws Exception
    {
        ServerSocket gameSocket = new ServerSocket(8686);
        System.out.println("Server running...");
        String login;

        for (int i = 0; i < 5; i++)
        {
            players[i] = gameSocket.accept();
            is[i] = new BufferedReader(new InputStreamReader(players[i].getInputStream()));
            os[i] = new DataOutputStream(players[i].getOutputStream());
            os[i].writeBytes("CONNECT\n");
            System.out.println("CONNECT");
            login = is[i].readLine();
            while (login.equals("")) {
                login = is[i].readLine();
            }
            System.out.println(login);
            os[i].writeBytes("OK\n");
            System.out.println("OK");
            login = login.split("\\s+")[1];
            logins[i] = login;
        }

        for (int j = 0; j < 5; j++) {


            for (int i = 0; i < 5; i++) {
                if (Server.round != 1) {
                    Server.os[i].writeBytes(String.format("ROUND %d\n", Server.round));
                    System.out.println(String.format("ROUND %d", Server.round));
                } else {
                    Server.os[i].writeBytes(String.format("START %d\n", i + 1));
                    System.out.println(String.format("START %d", i + 1));
                }
                Random rand = new Random();
                ///currentX[i] = rand.nextInt(50);
                ///currentY[i] = rand.nextInt(50);
                currentX[i] = 20 + i;
                currentY[i] = 20 - i;
                System.out.println(currentX[i]);
                System.out.println(currentY[i]);
                board[currentX[i]][currentY[i]] = i + 1;
                System.out.println(String.format("Player: %d Login: %s Position: (%d,%d)", i, logins[i], currentX[i], currentY[i]));
                Server.os[i].writeBytes(String.format("PLAYERS (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", Server.currentX[0], Server.currentY[0], Server.currentX[1], Server.currentY[1], Server.currentX[2], Server.currentY[2], Server.currentX[3], Server.currentY[3], Server.currentX[4], Server.currentY[4]));
                System.out.println(String.format("PLAYERS (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d)", Server.currentX[0], Server.currentY[0], Server.currentX[1], Server.currentY[1], Server.currentX[2], Server.currentY[2], Server.currentX[3], Server.currentY[3], Server.currentX[4], Server.currentY[4]));
                Server.currentDirections[i] = Server.is[i].readLine().split("\\s+")[1];
                System.out.println(Server.currentDirections[i]);
                if (Server.currentDirections[i].equals("N")) {
                    if ((Server.currentX[i] - 1 < 0) || (Server.board[Server.currentX[i] - 1][Server.currentY[i]] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = i + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[i].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[i].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                            playersInactive[i] = 1;
                        }
                        Server.positionIdx--;
                        for (int z = 0; z < 50; z++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[z][k]));
                            }
                            System.out.println();
                        }
                        continue;
                    }
                    os[i].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[i] - 1][Server.currentY[i]] = i + 1;
                    Server.currentX[i] = Server.currentX[i] - 1;
                } else if (Server.currentDirections[i].equals("S")) {
                    if ((Server.currentX[i] + 1 > 49) || (Server.board[Server.currentX[i] + 1][Server.currentY[i]] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = i + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[i].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[i].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                            playersInactive[i] = 1;
                        }
                        Server.positionIdx--;
                        for (int z = 0; z < 50; z++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[z][k]));
                            }
                            System.out.println();
                        }
                        continue;
                    }
                    os[i].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[i] + 1][Server.currentY[i]] = i + 1;
                    Server.currentX[i] = Server.currentX[i] + 1;
                } else if (Server.currentDirections[i].equals("E")) {
                    if ((Server.currentY[i] + 1 > 49) || (Server.board[Server.currentX[i]][Server.currentY[i] + 1] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = i + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[i].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[i].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                            playersInactive[i] = 1;
                        }
                        Server.positionIdx--;
                        for (int z = 0; z < 50; z++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[z][k]));
                            }
                            System.out.println();
                        }
                        continue;
                    }
                    os[i].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[i]][Server.currentY[i] + 1] = i + 1;
                    Server.currentY[i] = Server.currentY[i] + 1;
                } else if (Server.currentDirections[i].equals("W")) {
                    if ((Server.currentY[i] - 1 < 0) || (Server.board[Server.currentX[i]][Server.currentY[i] - 1] != 0)) {
                        Server.positions[Server.round - 1][Server.positionIdx] = i + 1;
                        if (Server.positionIdx == 0) {
                            Server.os[i].writeBytes(String.format("WIN\n"));
                            System.out.println("WIN");
                        } else {
                            Server.os[i].writeBytes(String.format("LOST %d\n", Server.positionIdx + 1));
                            System.out.println(String.format("LOST %d", Server.positionIdx + 1));
                            playersInactive[i] = 1;
                        }
                        Server.positionIdx--;
                        for (int z = 0; z < 50; z++) {
                            for (int k = 0; k < 50; k++) {
                                System.out.print(String.format("%s ", Server.board[z][k]));
                            }
                            System.out.println();
                        }
                        continue;
                    }
                    os[i].writeBytes("OK\n");
                    System.out.println("OK");
                    Server.board[Server.currentX[i]][Server.currentY[i] - 1] = i + 1;
                    Server.currentY[i] = Server.currentY[i] - 1;
                }
                threads[i] = new Client(players[i], i);
                threads[i].start();
            }

            for (int k = 0; k < 5; k++) {
                threads[k].join();
            }

            Server.board = new int[50][50];
            Server.currentDirections = new String[5];
            Server.playersInactive = new int[5];
            currentX = new int[5];
            currentY = new int[5];
            positionIdx = 4;
            Server.round++;

        }

        int[] winners = new int[5];

        for (int l = 0; l < 5; l++) {

            int sum = 0;

            for (int m = 0; m < 5; m++) {
                for (int i = 0; i < 5; i++) {
                    if (positions[m][i] == l + 1) {
                        sum = sum + (i + 1);
                        break;
                    }
                }


            }
            winners[l] = sum;
        }

        int[] winnersList = new int[5];
        int minValue = winners[0];
        int winner = 0;

        for (int j = 0; j < 5; j++) {
            for (int i = 1; i < 5; i++) {

                if (winners[i] < minValue) {
                    minValue = winners[i];
                    winner = i;
                }

            }
            minValue = winners[0];
            winnersList[j] = winner;
            winners[winner] = 100;
        }

        for (int i = 0; i < 5; i++) {
            Server.os[i].writeBytes(String.format("KONIEC %s %s %s %s %s\n", Server.logins[winnersList[0]], Server.logins[winnersList[1]], Server.logins[winnersList[2]], Server.logins[winnersList[3]], Server.logins[winnersList[4]]));
            System.out.println(String.format("KONIEC %s %s %s %s %s", Server.logins[winnersList[0]], Server.logins[winnersList[1]], Server.logins[winnersList[2]], Server.logins[winnersList[3]], Server.logins[winnersList[4]]));
        }
        gameSocket.close();




    }
}


