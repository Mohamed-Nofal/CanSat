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
       

    }

}
