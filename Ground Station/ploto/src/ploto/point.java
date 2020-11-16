/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ploto;

/**
 *
 * @author Mohamed
 */
public class point {
    private double x;
    private double y;

    public point(double x, double y) {
        this.x = x;
        this.y = y;
    }
 

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }
public void setX(double x) {
      this.x = x;
   }
   public void setY(double y) {
      this.y = y;
   } 

     @Override
    public String toString() {
        return ("(" + x + "," + y + ")"); 
    }
    
}
