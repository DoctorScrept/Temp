using UnityEngine;
using System.Collections;

public class CurveTracer3DControler : MonoBehaviour
{
	public TracerDevice device;
	public Plotter plotter;

	void Start () {
	
	}

	void Update ()
	{
		float[] data = device.GetData();
		if (data != null) {
		    Surface s = new Surface(4, 4);
			s.SetData(data);
			plotter.DrawToSlot(0, s);
		}
	}

	public void StatMeasure()
	{
		if (!device.StatMeasure()) {
			Debug.LogError(device.GetErrorText());
		}
	}

	public void StopMeasure()
	{
		device.StopMeasure();
	}
}
