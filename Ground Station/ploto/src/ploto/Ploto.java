/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ploto;

import javafx.application.Application;
import javafx.event.Event;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.shape.Cylinder;
import javafx.scene.shape.Sphere;

import javafx.stage.Stage;

/**
 *
 * @author Mohamed
 */
public class Ploto extends Application {
    
    
    public void start(Stage stage) throws Exception {
        
        
        
        
                
        
        
        
        
        
        
        
        
        
        
        Parent root = FXMLLoader.load(getClass().getResource("FXMLDocument.fxml"));
        Scene scene = new Scene(root);
        scene.getStylesheets().add(Ploto.class.getResource("styles.css").toExternalForm());
        stage.setTitle("sstl ploto");
        stage.setScene(scene);
        stage.show();
    }

    /**
     * @param args the command line arguments
     */
  
    public static void main(String[] args) {
        launch(args);
        final PhongMaterial redMaterial = new PhongMaterial();
       redMaterial.setSpecularColor(Color.ORANGE);
       redMaterial.setDiffuseColor(Color.RED);
 
       final PhongMaterial blueMaterial = new PhongMaterial();
       blueMaterial.setDiffuseColor(Color.BLUE);
       blueMaterial.setSpecularColor(Color.LIGHTBLUE);

       final PhongMaterial greyMaterial = new PhongMaterial();
       greyMaterial.setDiffuseColor(Color.DARKGREY);
       greyMaterial.setSpecularColor(Color.GREY);

       final Box red = new Box(400, 400, 400);
       red.setMaterial(redMaterial);
 
       final Sphere blue = new Sphere(200);
       blue.setMaterial(blueMaterial);
 
       final Cylinder grey = new Cylinder(5, 100);
       grey.setMaterial(greyMaterial);
       

    }

}
