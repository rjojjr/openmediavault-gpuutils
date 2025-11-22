<?php

class OMVRpcServiceNvidiaUtil extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "NvidiaUtil";
    }

    public function initialize()
    {
        $this->registerMethod("gputemp");
        $this->registerMethod("gpuutil");
        $this->registerMethod("gpumemutil");
        $this->registerMethod("gpupowerutil");
    }

    public function gputemp($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuTemp = \OMV\Environment::get("OMV_NVIDIA_GPU_TEMP_COMMAND",
            "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "gputemp" => intval($output[0])
        ];
    }

    public function gpuutil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_UTIL_COMMAND",
            "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuUtil);
        $cmd->execute($output, $exitStatus);
        return [
            "gpuutil" => intval($output[0])
        ];
    }

    public function gpupowerutil($param, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $gpuUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_POWER_COMMAND",
            "nvidia-smi --query-gpu=power.draw,power.limit --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuUtil);
        $cmd->execute($output, $exitStatus);
        $splittedOutput = explode(" ", $output[0]);
        return [
            "gpuPowerDraw" => intval($splittedOutput[0]),
            "gpuPowerLimit" => intval($splittedOutput[1])
        ];
    }

    public function gpumemutil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuMemUtil = \OMV\Environment::get("OMV_NVIDIA_GPU_MEM_UTIL_COMMAND",
            "nvidia-smi --query-gpu=memory.total,memory.free --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuMemUtil);
        $cmd->execute($output, $exitStatus);
        $splittedOutput = explode(" ", $output[0]);
        return [
            "gpuMemTotal" => intval($splittedOutput[0])*1048576, // 1048576 to convert MiB to Bytes
            "gpuMemAvailable" => intval($splittedOutput[1])*1048576
        ];
    }
}