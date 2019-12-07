import java.io.*;
import java.util.*;

public class Solution {
    static class Graph {
        private int[] colors;
        private LinkedList<Integer>[] adjList;
        private int totalNodes;

        public void loadGraph() {
            Scanner input = new Scanner(System.in);
            totalNodes = input.nextInt();
            int totalEdges = input.nextInt();

            adjList = new LinkedList[totalNodes];
            colors = new int[totalNodes];

            for (int i = 0; i < totalNodes; i++) {
                adjList[i] = new LinkedList<Integer>();
            }

            for (int i = 0; i < totalEdges; i++) {
                int firstNode = input.nextInt() - 1;
                int secondNode = input.nextInt() - 1;
                adjList[firstNode].add(secondNode);
                adjList[secondNode].add(firstNode);
            }

            for (int i = 0; i < totalNodes; i++) {
                colors[i] = input.nextInt();
            }
            int dist = nearestColor(input.nextInt());
            System.out.println(dist == Integer.MAX_VALUE ? -1 : dist);
            input.close();
        }

        public int nearestColor(int color) {
            int[] distances = new int[totalNodes];
            boolean[] visited = new boolean[totalNodes];
            int minDist = Integer.MAX_VALUE;

           for (int i = 0; i < totalNodes; i++) {
               if (colors[i] != color) {
                   continue;
               }

               for (int j = 0; j < totalNodes; j++) {
                   distances[j] = Integer.MAX_VALUE;
                   visited[j] = false;
               }
               distances[i] = 0;

               Queue<Integer> toVisit = new LinkedList<Integer>();
               boolean foundMin = false;

               toVisit.add(i);

               while (!toVisit.isEmpty() && !foundMin) {
                   int currNode = toVisit.poll();
                   visited[currNode] = true;

                   if (distances[currNode] > minDist) {
                       break;
                   }
                   LinkedList<Integer> neighborList = adjList[currNode];

                   for (Integer neighbor : neighborList) {
                       if (!visited[neighbor]) {
                           distances[neighbor] = Math.min(distances[neighbor], distances[currNode] + 1);

                           if (colors[neighbor] == color && distances[neighbor] < minDist) {
                               minDist = distances[neighbor];
                               foundMin = true;
                               break;
                           }

                           toVisit.add(neighbor);
                       }
                   }
               }
           } 

           return minDist;
        }
    }
    public static void main(String[] args) {
        Graph g = new Graph();
        g.loadGraph();
    }
}