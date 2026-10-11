# EcoSort: Smart Waste Management System

EcoSort is an automated waste-segregation bin that sorts waste into **metallic, wet and dry** compartments without manual handling. Improper segregation at the source contaminates recyclables, fills landfills and exposes sanitation workers to hazardous material. EcoSort combines **computer vision (YOLO object detection)** with **low-cost physical sensors** and a **motor-driven sorting mechanism**, all coordinated by an Arduino Uno.


## How it works

1. **Detect:** a proximity sensor and an IR sensor confirm that an item has been dropped into the input slot.
2. **Classify:** the item is identified as metallic, wet or dry. Metal is picked up by the proximity sensor, and moisture is measured by a rain-drop/moisture sensor. A webcam with a **YOLOv11** model adds visual identification, and the project paper describes a weighted decision that combines the vision and sensor results.
3. **Sort:** a **stepper motor** rotates the internal platform to align the right compartment, and a **servo-controlled flap gate** opens to drop the item in. The platform then returns to its home position.

```
 Item dropped in
        │
        ▼
 Proximity / IR sensors ──► object present?
        │
        ├── Metal detected ───────────────► rotate platform 240° → open flap → close → return
        │
        └── IR triggered → wait 0.5 s → average 3 moisture readings (0–100)
                 ├── moisture > 20 (wet) ──► rotate platform 120° → open flap → close → return
                 └── moisture ≤ 20 (dry) ──► flap opens at home position (0°) → close
```

## Hardware

| Component | Role | Arduino pin |
|---|---|---|
| Arduino Uno | Central controller | n/a |
| Proximity sensor | Metal detection (reads LOW when metal is present) | D6 |
| IR sensor | Detects an object in the input slot (reads LOW when blocked) | D5 |
| Rain-drop / moisture sensor | Wet waste detection (analog) | A0 |
| Servo motor | Flap gate (70° closed, 180° open) | D7 |
| Stepper motor (CheapStepper library) | Rotates the platform to the target compartment | D8, D9, D10, D11 |
| Webcam + laptop | Vision-based waste detection (YOLOv11) | USB |
| Segmented waste bin | Metallic / wet / dry compartments | n/a |

## Firmware (`ipd_final.ino`)

- Libraries: `CheapStepper`, `Servo` (install CheapStepper from the Arduino Library Manager).
- Stepper speed is set to 17 RPM. Serial output runs at **9600 baud** and prints `Metal detected`, `Moisture: <value>`, `Wet Waste` or `Dry Waste` for debugging.
- **Moisture calibration:** each reading is clamped to 485–1023 and mapped to 100–0 (a lower raw value means wetter). Three samples 75 ms apart are averaged, and a result above **20** counts as wet waste. Adjust the `485`, `1023` and `20` values to recalibrate for your sensor.
- Metal detection has priority: the proximity sensor is checked before the IR/moisture path.
- The sketch implements the sensor-driven sorting stage. It does not read YOLO results over the serial port.

### Setup

1. Wire the components as in the pin table above.
2. Open `ipd_final.ino` in the Arduino IDE and install the **CheapStepper** library.
3. Select **Arduino Uno** and the correct port, then upload.
4. Open the Serial Monitor at 9600 baud. You should see `System Ready...`.
5. Drop a test item in: metal for the metal path, a damp item for wet waste, and a dry item for dry waste.

## Vision module

The vision side uses a webcam placed near the input slot and a **YOLO** object detection model (the project report explores YOLOv8n and the final paper uses YOLOv11) that outputs a class label and confidence score for each detected item. The model was trained on a custom-labelled waste image dataset (annotated in Label Studio, trained in Google Colab) and tested live with an OpenCV webcam pipeline. Detections are shown as annotated frames with bounding boxes, class names and confidence.

## Results and limitations

- The prototype showed that vision-based classification can be combined with a motorised sorter, and it directed items into the right compartment from the model's predictions.
- Performance depends on lighting, camera placement and the quality of the trained model, and mixed or partly visible items reduce accuracy.
- The project did not report a final numerical accuracy figure. Testing was qualitative, in controlled indoor conditions.

## Future scope

- Larger, more varied dataset (transparent, reflective and deformable waste).
- Edge deployment on a Jetson Nano or Raspberry Pi so the bin runs without a laptop.
- IoT connectivity: ultrasonic fill-level sensing, load cells and gas sensors, with a cloud dashboard.
- Robotic or soft-gripper sorting for irregular items.
- Large-scale trials in campuses, residential societies and recycling centres.

## Applications

Smart bins and recycling workflows · municipal solid-waste operations · industrial and commercial waste handling · campuses and residential complexes · education and research testbed · smart-city waste networks.

## Tech stack

Arduino (C++) · CheapStepper · Servo · YOLOv11 / Ultralytics · Python · OpenCV · Label Studio · Google Colab

## Authors

- **Shrutik Acharya**
- **Soham Bhosale**
- **Rugved Mane**
- **Siddhaant Pandit**

## References

1. A. B. Wahyutama and M. Hwang, "YOLO-Based Object Detection for Separate Collection of Recyclables and Capacity Monitoring of Trash Bins," *Electronics*, vol. 11, no. 9, p. 1323, 2022. doi: 10.3390/electronics11091323
2. M. H. Moktar, H. Mohamed, S. S. H. Hajjaj and M. Z. Baharuddin, "Medical Waste Sorting Machine Development with IoT and YOLO Model Utilization," *Journal of Engineering and Applied Science*, vol. 72, no. 1, 2025. doi: 10.1186/s44147-025-00661-5
