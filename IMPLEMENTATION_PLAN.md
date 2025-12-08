# Implementation Plan: Nvidia GPUs Dashboard Widget

## Overview
Create a table variant OMV dashboard widget named "Nvidia GPUs" that displays all available metrics for every Nvidia GPU in the system.

## Steps

1. **Enhance the NvidiaUtil service to properly support multiple GPUs**
   - Modify the `getAllGpuMetrics` method in `/code/openmediavault-gpuutils/usr/share/openmediavault/engined/rpc/nvidia-util.inc`
   - Update the method to return an array of metrics for all GPUs instead of just one
   - Add proper GPU enumeration using nvidia-smi to get metrics for each GPU

2. **Create the new dashboard widget configuration file**
   - Create `/code/openmediavault-gpuutils/usr/share/openmediavault/workbench/dashboard.d/nvidia_gpus.yaml`
   - Define the widget with type: dashboard-widget and type: datatable
   - Set the title to "Nvidia GPUs"
   - Configure the data request to use the enhanced `getAllGpuMetrics` method
   - Define columns for all available GPU metrics

3. **Update the NvidiaUtil service to properly handle multiple GPUs**
   - Modify the `getAllGpuMetrics` method to return an array of all GPU metrics
   - Use nvidia-smi with appropriate query parameters to get metrics for each GPU
   - Ensure each GPU's metrics are properly structured for display

4. **Test the implementation**
   - Verify the widget displays correctly in the OMV dashboard
   - Confirm all GPU metrics are shown properly in the table
   - Test that the widget updates automatically as metrics change

## File Modifications

### File: `/code/openmediavault-gpuutils/usr/share/openmediavault/engined/rpc/nvidia-util.inc`
- Add logic to enumerate all GPUs using `nvidia-smi --query-gpu=index --format=csv,noheader,nounits`
- Modify `getAllGpuMetrics` to return array of all GPU metrics
- Ensure proper data structure for multiple GPUs

### File: `/code/openmediavault-gpuutils/usr/share/openmediavault/workbench/dashboard.d/nvidia_gpus.yaml`
- Create new YAML file with proper OMV dashboard widget schema
- Define widget as type: dashboard-widget with type: datatable
- Set appropriate title and permissions
- Configure request to use NvidiaUtil service and getAllGpuMetrics method
- Define all columns for GPU metrics

## Expected Output

A new "Nvidia GPUs" dashboard widget that displays a table showing all available metrics for every Nvidia GPU in the system, including:
- GPU ID
- Utilization
- Power consumption
- Memory usage
- Fan speed
- Temperature

The widget will automatically update every 5 seconds to show current metrics.