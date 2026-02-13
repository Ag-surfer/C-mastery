---
name: excalidraw-diagrams
description: Generate Excalidraw memory layout diagrams as JSON files. Auto-loads when creating .excalidraw files.
globs: ["**/*.excalidraw"]
---

## Color Palette
| Element | Stroke | Background |
|---|---|---|
| Stack | #d32f2f | #ffebee |
| Heap | #388e3c | #e8f5e9 |
| Code/Text | #1976d2 | #e3f2fd |
| Data/BSS | #f57c00 | #fff3e0 |
| Registers | #7b1fa2 | #f3e5f5 |
| Padding | #9e9e9e | #f5f5f5 |
| Danger/UB | #d32f2f | #ffcdd2 |
| Pointers | #333333 | — |

## Element Pattern
Each visible element needs TWO JSON objects: a shape + a bound text label.
````json
{
  "type": "rectangle",
  "id": "stack-frame-main",
  "x": 100, "y": 100, "width": 200, "height": 80,
  "strokeColor": "#d32f2f",
  "backgroundColor": "#ffebee",
  "fillStyle": "solid",
  "strokeWidth": 2,
  "roughness": 1,
  "boundElements": [{"type": "text", "id": "stack-frame-main-text"}]
}
````
````json
{
  "type": "text",
  "id": "stack-frame-main-text",
  "x": 120, "y": 125, "width": 160, "height": 30,
  "text": "main() frame",
  "fontSize": 16,
  "containerId": "stack-frame-main"
}
````

## Arrow Pattern (for pointers)
````json
{
  "type": "arrow",
  "id": "ptr-arrow-1",
  "x": 300, "y": 140, "width": 100, "height": 0,
  "strokeColor": "#333333",
  "strokeWidth": 2,
  "elbowed": true,
  "startBinding": {"elementId": "source-id"},
  "endBinding": {"elementId": "target-id"}
}
````

## File Structure
````json
{
  "type": "excalidraw",
  "version": 2,
  "elements": [],
  "appState": {"viewBackgroundColor": "#ffffff"}
}
````
