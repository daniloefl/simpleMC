package SimpleMC3d.client;

import thothbot.parallax.core.client.*;
import thothbot.parallax.core.shared.cameras.*;
import thothbot.parallax.core.shared.objects.*;
import thothbot.parallax.core.shared.materials.*;
import thothbot.parallax.core.shared.geometries.*;
import thothbot.parallax.core.shared.math.Color;

class SimpleMC3dScene extends AnimatedScene {

   PerspectiveCamera camera;
   private Mesh mesh;

   @Override
   protected void onStart() {
       // Loads default camera for the Animation
       camera = new PerspectiveCamera(
                                      70, // field of view
                                      getRenderer().getAbsoluteAspectRation(), // aspect ratio 
                                      1, // near
                                      1000 // far 
                                     );

       camera.getPosition().setZ(400);

       BoxGeometry geometry = new BoxGeometry( 200, 200, 200 );

       MeshBasicMaterial material = new MeshBasicMaterial();
       material.setColor( new Color(0xFF0000) );
       material.setWireframe( true );

       this.mesh = new Mesh(geometry, material);
       getScene().add(mesh);
   }

   @Override
   protected void onUpdate(double duration) {
       // Called when the animation should be updated.
       this.mesh.getRotation().addX(0.005);
       this.mesh.getRotation().addY(0.01);

       getRenderer().render(getScene(), camera);
   }
}

