<?php

class OMVRpcServiceNvidiaUtil extends \OMV\Rpc\ServiceAbstract
{
//    protected function loadConfigFile($filename)
//    {
//        $configFilePath = "/usr/share/openmediavault/workbench/dashboard.d/" . $filename;
//        return yaml_parse_file($configFilePath);
//    }
//
//    protected function getWidgetConfig($widgetId)
//    {
//        $filename = sprintf('%s-config.yaml', $widgetId);
//        return $this->loadConfigFile($filename);
//    }

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
        return [
            "gpuUtil" => intval($output[0])
        ];
    }

    public function getGpuPowerUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuPowerUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_POWER_COMMAND",
            "nvidia-smi --query-gpu=power.draw,power.limit --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuPowerUtil);
        $cmd->execute($output, $exitStatus);
        $splittedOutput = explode(",", $output[0]);
        return [
            "gpuPowerDraw" => intval($splittedOutput[0]),
            "gpuPowerLimit" => intval($splittedOutput[1])
        ];
    }

    public function getGpuMemUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuMemUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_MEM_UTIL_COMMAND",
            "nvidia-smi --query-gpu=memory.total,memory.free,memory.used --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuMemUtil);
        $cmd->execute($output, $exitStatus);
        $splittedOutput = explode(" ", $output[0]);
        return [
            "gpuMemTotal" => intval($splittedOutput[0])*1048576, // 1048576 to convert MiB to Bytes
            "gpuMemAvailable" => intval($splittedOutput[1])*1048576,
            "gpuMem" => intval($splittedOutput[2])*1048576
        ];
    }

    public function getGpuFanUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuFanUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_FAN_UTIL_COMMAND",
            "nvidia-smi --query-gpu=fan.speed --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuFanUtil);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuFanUtil" => intval($output[0])
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
        $gpuFan = $this->getGpuFanUtil($param, $context)['gpuFanUtil'];

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