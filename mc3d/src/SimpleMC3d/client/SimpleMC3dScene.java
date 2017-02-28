package SimpleMC3d.client;

import thothbot.parallax.core.client.*;
import thothbot.parallax.core.client.events.*;
import thothbot.parallax.core.shared.lights.*;
import thothbot.parallax.core.shared.cameras.*;
import thothbot.parallax.core.shared.objects.*;
import thothbot.parallax.core.shared.materials.*;
import thothbot.parallax.core.shared.geometries.*;
import thothbot.parallax.core.shared.math.Color;
import thothbot.parallax.core.shared.math.*;

class SimpleMC3dScene extends AnimatedScene {

  int screenWidth = 800, screenHeight = 600;
  PerspectiveCamera camera;
  private Mesh mesh;
  DirectionalLight light;
  private MeshBasicMaterial material1;
  private MeshBasicMaterial material2;

  SphereGeometry beam1;
  SphereGeometry beam2;

  Mesh beam1m;
  Mesh beam2m;

  double time;
  MCEvent e;

  @Override
  protected void onStart() {
    // Loads default camera for the Animation
    screenWidth = getRenderer().getAbsoluteWidth();
    screenHeight = getRenderer().getAbsoluteHeight();
    camera = new PerspectiveCamera(
                                   20, // field of view
                                   getRenderer().getAbsoluteAspectRation(), // aspect ratio 
                                   1, // near
                                   10000 // far 
                                  );


    light = new DirectionalLight(0xffffff);
    light.getPosition().set(0, 0, 1);
    getScene().add(light);

    beam1 = new SphereGeometry(10);
    beam2 = new SphereGeometry(10);

    material1 = new MeshBasicMaterial();
    material1.setColor( new Color(0x00FF00) );
    material1.setWireframe( true );

    material2 = new MeshBasicMaterial();
    material2.setColor( new Color(0xFF0000) );
    material2.setWireframe( true );

    beam1m = new Mesh(beam1, material1);
    beam1m.getPosition().setX(300);
    beam1m.getPosition().setY(0);
    beam1m.getPosition().setZ(0);
    getScene().add(beam1m);

    beam2m = new Mesh(beam2, material2);
    beam2m.getPosition().setX(-300);
    beam2m.getPosition().setY(0);
    beam2m.getPosition().setZ(0);
    getScene().add(beam2m);

    /*
    camera.getPosition().setX(-500);
    camera.getPosition().setY(-500);
    camera.getPosition().setZ(1800);
    camera.lookAt(getScene().getPosition());
    */

    time = 9;
  }

  @Override
  protected void onUpdate(double duration) {
    // Called when the animation should be updated.
    //this.mesh.getRotation().addX(0.005);
    //this.mesh.getRotation().addY(0.01);
    //camera.getPosition().addX(( - mouseX - camera.getPosition().getX()) * 0.05 );
    //camera.getPosition().addY(( mouseY - camera.getPosition().getY()) * 0.05 );

    double infinity = 300;
    double com = Math.abs(e.pz[0]);
    double dt = 0.01;
    if (time < 0.5) {
      double b1x = 0;
      double b1y = 0;
      double b1z = +infinity;
      double b2x = 0;
      double b2y = 0;
      double b2z = -infinity;
      double t = infinity/com*time/0.5;
      beam1m.getPosition().setZ(b1x+e.px[0]*t);
      beam1m.getPosition().setY(b1y+e.py[0]*t);
      beam1m.getPosition().setX(b1z+e.pz[0]*t);
      beam2m.getPosition().setZ(b2x+e.px[1]*t);
      beam2m.getPosition().setY(b2y+e.py[1]*t);
      beam2m.getPosition().setX(b2z+e.pz[1]*t);
      time += dt;
    } else if (time < 1) {
      double t = infinity/com*(time-0.5)/0.5;
      double b1x = 0;
      double b1y = 0;
      double b1z = 0;
      double b2x = 0;
      double b2y = 0;
      double b2z = 0;
      beam1m.getPosition().setZ(b1x+e.px[2]*t);
      beam1m.getPosition().setY(b1y+e.py[2]*t);
      beam1m.getPosition().setX(b1z+e.pz[2]*t);
      beam2m.getPosition().setZ(b2x+e.px[3]*t);
      beam2m.getPosition().setY(b2y+e.py[3]*t);
      beam2m.getPosition().setX(b2z+e.pz[3]*t);
      time += dt;
    } else {
    }

    getRenderer().render(getScene(), camera);
  }

  public void addInScene(MCEvent _e) {
    e = _e;
    time = 0;
    getRenderer().render(getScene(), camera);
  }
}

