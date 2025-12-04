<?php

class OMVRpcServiceNvidiaUtil extends OMVRpcServiceAbstract
{
    protected function loadConfigFile($filename)
    {
        $configFilePath = "/usr/share/openmediavault/workbench/dashboard.d/" . $filename;
        return yaml_parse_file($configFilePath);
    }

    protected function getWidgetConfig($widgetId)
    {
        $filename = sprintf('%s-config.yaml', $widgetId);
        return $this->loadConfigFile($filename);
    }

    public function getName()
    {
        return "NvidiaUtil";
    }

    public function initialize()
    {
        $this->registerMethod("getGpuTemp");
        $this->registerMethod("getGpuUtil");
        $this->registerMethod("getGpuMemUtil");
        $this->registerMethod("getGpuPowerUtil");
        $this->registerMethod("getGpuFanUtil");
        $this->registerMethod("getAllGpuMetrics");
    }

    public function getGpuTemp($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuTemp = \OMV\Environment::get("OMV_NVIDIA_GPU_TEMP_COMMAND",
            "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuTemp" => intval($output[0])
        ];
    }

    public function getGpuUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_UTIL_COMMAND",
            "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuUtil);
        $cmd->execute($output, $exitStatus);
        $widgetConfig = $this->getWidgetConfig('nvidia-gpu-util');
        if (isset($widgetConfig['tuning'])) {
            $threshold = $widgetConfig['tuning']['threshold'];
            $currentUtil = intval($output[0]);
            if ($currentUtil >= $threshold) {
                // Add logic to handle high utilization (e.g., trigger an alert, optimize performance)
                error_log(sprintf('GPU Utilization Alert: Current Utilization is %d which exceeds the threshold of %d', $currentUtil, $threshold));
            }
        }
        return [
            "gpuUtil" => $currentUtil
        ];
    }

    public function getGpuPowerUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuPower = \OMV\Environment::get("OMV_NVIDIA_GPU_POWER_COMMAND",
            "nvidia-smi --query-gpu=power.draw --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuPower);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuPower" => intval($output[0])
        ];
    }

    public function getGpuMemUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuMem = \OMV\Environment::get("OMV_NVIDIA_GPU_MEM_COMMAND",
            "nvidia-smi --query-gpu=memory.used --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuMem);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuMem" => intval($output[0])
        ];
    }

    public function getGpuFanUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuFan = \OMV\Environment::get("OMV_NVIDIA_GPU_FAN_COMMAND",
            "nvidia-smi --query-gpu=fan.speed --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuFan);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuFan" => intval($output[0])
        ];
    }

    public function getAllGpuMetrics($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        
        // Get all GPU metrics
        $gpuTemp = $this->getGpuTemp($param, $context)['gpuTemp'];
        $gpuUtil = $this->getGpuUtil($param, $context)['gpuUtil'];
        $gpuPower = $this->getGpuPowerUtil($param, $context)['gpuPower'];
        $gpuMem = $this->getGpuMemUtil($param, $context)['gpuMem'];
        $gpuFan = $this->getGpuFanUtil($param, $context)['gpuFan'];

        // Assuming a single GPU for simplicity, in case of multiple GPUs, we need to handle that
        return [
            'gpuId' => 0, // Placeholder for GPU ID
            'gpuTemp' => $gpuTemp,
            'gpuUtil' => $gpuUtil,
            'gpuPower' => $gpuPower,
            'gpuMem' => $gpuMem,
            'gpuFan' => $gpuFan
        ];
    }
}