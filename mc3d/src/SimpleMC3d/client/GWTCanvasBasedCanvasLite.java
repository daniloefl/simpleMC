package SimpleMC3d.client;
import com.googlecode.gchart.client.GChartCanvasLite;

import com.google.gwt.widgetideas.graphics.client.GWTCanvas; 
import com.google.gwt.widgetideas.graphics.client.Color; 

public class GWTCanvasBasedCanvasLite extends GWTCanvas implements GChartCanvasLite {
  public void setStrokeStyle(String cssColor) {
    // Sharp angles of default MITER can overwrite adjacent pie slices
    setLineJoin(GWTCanvas.ROUND);
    setStrokeStyle(new Color(cssColor));
  }
  public void setFillStyle(String cssColor) {
    setFillStyle(new Color(cssColor));
  }
}

