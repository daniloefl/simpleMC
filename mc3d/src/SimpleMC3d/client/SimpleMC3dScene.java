package SimpleMC3d.client;

import thothbot.parallax.core.client.*;
import thothbot.parallax.core.client.events.*;
import thothbot.parallax.core.shared.lights.*;
import thothbot.parallax.core.shared.cameras.*;
import thothbot.parallax.core.shared.objects.*;
import thothbot.parallax.core.shared.core.*;
import thothbot.parallax.core.shared.materials.*;
import thothbot.parallax.core.shared.geometries.*;
import thothbot.parallax.core.shared.math.Color;
import thothbot.parallax.core.shared.math.*;
import thothbot.parallax.core.shared.helpers.*;

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

  ArrowHelper l1;
  ArrowHelper l2;
  ArrowHelper l3;
  ArrowHelper l4;

  AxisHelper axis;

  double time;
  MCEvent e;

  double infinity = 400;

  @Override
  protected void onStart() {
    // Loads default camera for the Animation
    screenWidth = getRenderer().getAbsoluteWidth();
    screenHeight = getRenderer().getAbsoluteHeight();
    camera = new PerspectiveCamera(
                                   45, // field of view
                                   getRenderer().getAbsoluteAspectRation(), // aspect ratio 
                                   1, // near
                                   10000 // far 
                                  );

    getScene().add( new AmbientLight( 0x404040 ) );
    light = new DirectionalLight( 0xffffff );
    light.getPosition().set( 0, 100, 0 );
    getScene().add( light );

    beam1 = new SphereGeometry(20);
    beam2 = new SphereGeometry(20);

    material1 = new MeshBasicMaterial();
    material1.setColor( new Color(0x0000FF) );
    //material1.setWireframe( true );

    material2 = new MeshBasicMaterial();
    material2.setColor( new Color(0xFF0000) );
    //material2.setWireframe( true );

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

    axis = new AxisHelper();
    axis.getPosition().set( 0, 0, 0 );
    getScene().add(axis);

    l1 = new ArrowHelper(new Vector3(0, 0, 0), new Vector3(infinity, 0, 0));
    l2 = new ArrowHelper(new Vector3(0, 0, 0), new Vector3(-infinity, 0, 0));
    l3 = new ArrowHelper(new Vector3(1, 1, 1), new Vector3(0, 0, 0));
    l4 = new ArrowHelper(new Vector3(1, 1, 1), new Vector3(0, 0, 0));
    l1.setColor(0x0000ff);
    l2.setColor(0xff0000);
    l3.setColor(0x0000ff);
    l4.setColor(0xff0000);

    getScene().add(l1);
    getScene().add(l2);
    getScene().add(l3);
    getScene().add(l4);

    camera.getPosition().setZ(1500);
    camera.lookAt(getScene().getPosition());


    time = 9;
  }

  @Override
  protected void onUpdate(double duration) {
    // Called when the animation should be updated.
    //camera.getPosition().setX(Math.cos( duration * 0.0001 ) * 200.0);
    //camera.getPosition().setY(Math.sin( duration * 0.0001 ) * 200.0);

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

      l1.setLength(Math.abs(b1z+e.pz[0]*t-infinity));
      l2.setLength(Math.abs(b2z+e.pz[1]*t-(-infinity)));
      l3.setLength(0);
      l4.setLength(0);

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

      l1.setLength(infinity);
      l2.setLength(infinity);
      l3.setLength(Math.sqrt(Math.pow(e.px[2]*t,2) + Math.pow(e.py[2]*t, 2) + Math.pow(e.pz[2]*t, 2)));
      l4.setLength(Math.sqrt(Math.pow(e.px[3]*t,2) + Math.pow(e.py[3]*t, 2) + Math.pow(e.pz[3]*t, 2)));

      time += dt;
    } else {
    }

    getRenderer().render(getScene(), camera);
  }

  public void addInScene(MCEvent _e) {
    e = _e;
    time = 0;

    l1.setDirection(new Vector3(-1, 0, 0));
    l2.setDirection(new Vector3(1, 0, 0));
    double norm3 = Math.sqrt(Math.pow(e.px[2],2) + Math.pow(e.py[2], 2) + Math.pow(e.pz[2], 2));
    double norm4 = Math.sqrt(Math.pow(e.px[3],2) + Math.pow(e.py[3], 2) + Math.pow(e.pz[3], 2));
    l3.setDirection(new Vector3(e.pz[2]/norm3, e.py[2]/norm3, e.px[2]/norm3));
    l4.setDirection(new Vector3(e.pz[3]/norm4, e.py[3]/norm4, e.px[3]/norm4));
    getRenderer().render(getScene(), camera);
  }
}

