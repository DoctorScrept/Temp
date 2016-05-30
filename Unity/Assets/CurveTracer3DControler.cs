using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

//«Файл бібліотеки CurveTracer3DDriver.dll не знайдено». 
//	"файл не знайдено"
public class CurveTracer3DControler : MonoBehaviour
{
	public TracerDevice device;
	public Plotter plotter;
	
	public MainMenu mainMenu;
	public ErrorDialog errorDialog;
	public TPS_RPG_CamCon cam;

	public Button startMeasureButton;
	public Text completeText;
	public MarkerControler markerControler;

	private Stack<Dialog> dialogs = new Stack<Dialog>();

	private int lastPersent = 0;
	private float[,] defaultData = { 	
		{0f, 0f, 0f, 0f},
		{0f, 0f, 0f, 0f},
		{0f, 0f, 2f, 0f},
		{0f, 0f, 0f, 1f}};
	//105
	//ГТ309

	private float[,] defaultData2 = { 	
		{0f, 0f, 0f, 0f, 0f, 0f, 0f, 0f, 0f, 0f, 0f},
		{0f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f},
		{0f, 0.5f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f},
		{0f, 0.8f, 0.9f, 0.91875f, 0.9375f, 0.95625f, 0.975f, 0.99375f, 1.0125f, 1.03125f, 1.05f},
		{0f, 1.3f, 1.5f, 1.55f, 1.6f, 1.65f, 1.7f, 1.75f, 1.8f, 1.85f, 1.9f},
		{0f, 1.6f, 2f, 2.0625f, 2.125f, 2.1875f, 2.25f, 2.3125f, 2.375f, 2.4375f, 2.5f},
		{0f, 1.2f, 2.5f, 2.6f, 2.7f, 2.8f, 2.9f, 3f, 3.1f, 3.2f, 3.3f},
		{0f, 2.7f, 3.1f, 3.225f, 3.35f, 3.475f, 3.6f, 3.725f, 3.85f, 3.975f, 4.1f},
		{0f, 3.3f, 3.7f, 4f, 4.2f, 4.4f, 4.6f, 4.8f, 5f, 5.2f, 5.4f},
		{0f, 4.1f, 4.7f, 5.0f, 5.2f, 5.4f, 5.6f, 5.8f, 6f, 6.2f, 6.4f},
		{0f, 4.2f, 5.0f, 5.5f, 5.771429f, 6.042857f, 6.314286f, 6.585714f, 6.857143f, 7.128572f, 7.4f}
	};


	void Awake() {
		Dialog.controler = this;
		mainMenu.markerControler.plotter = plotter;
	}

	void Start () {
		Surface s = new Surface(11, 11);
		s.SetData(defaultData2);
		Debug.Log(defaultData2.Length);

		plotter.DrawToSlot(0, s);
	}


	void Update ()
	{
		int complelte = device.GetPercentComplete();
		if (lastPersent != complelte) {
			lastPersent = complelte;
			completeText.text = complelte.ToString() + "%";
		}

		float[] data = device.GetData();
		if (data != null) {
		    Surface s = new Surface(4, 4);
			s.SetData(data);
			plotter.DrawToSlot(0, s);

			startMeasureButton.gameObject.SetActive(true);
			completeText.gameObject.SetActive(false);
			completeText.text = "0%";
			CloseAll();
		}
	}

	public void StatMeasure()
	{
		if (!device.StatMeasure()) {
			string errorText = device.GetErrorText();
			PushDialog(errorDialog);
			errorDialog.SetErrorText(errorText);
		} else {
			startMeasureButton.gameObject.SetActive(false);
			completeText.gameObject.SetActive(true);
		}
	}

	public void StatMeasureTest()
	{
		if (!device.StatMeasure()) {
			string errorText = device.GetErrorText();
			Debug.LogError(errorText);
			PushDialog(errorDialog);
			errorDialog.SetErrorText(errorText);
		}
	}
	
	public void StopMeasure() {
		device.StopMeasure();
	}
	
	public void OpenCloseMenu()
	{
		if (dialogs.Count > 0) {
			PopDialog();
		} else {
			PushDialog(mainMenu);
		}
	}


	public void PushDialog(Dialog dialog)
	{
		if (dialogs.Count > 0) {
			Dialog prev = dialogs.Peek();
			prev.SetIntarct(false);
		}

		dialogs.Push(dialog);
		dialog.gameObject.SetActive(true);

		UpdateAbility();
	}

	public Dialog PopDialog()
	{
		Dialog dialog = dialogs.Pop();
		dialog.gameObject.SetActive(false);

		if (dialogs.Count > 0) {
			Dialog prev = dialogs.Peek();
			prev.SetIntarct(true);
		}
		UpdateAbility();
		return dialog;
	}

	public void CloseAll() {
		while (dialogs.Count > 0) {
			PopDialog();
		}
	}

	private void UpdateAbility()
	{
		bool isEnable = dialogs.Count < 2;
		markerControler.IsActive = isEnable;
		cam.IsActive = isEnable;
	}
}
