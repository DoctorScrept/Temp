using UnityEngine;
using System.Collections;

public class SettingsDialog : Dialog {

	public GameObject axes;
	public GameObject grid;
	public MeshRenderer surfaceRenderer;

	public void ShowAxes(bool value)
	{
		axes.SetActive(value);
	}

	public void ShowGrid(bool value)
	{
		grid.SetActive(value);
	}

	public void ShowSurface(bool value)
	{
		surfaceRenderer.enabled = value;
	}

}
