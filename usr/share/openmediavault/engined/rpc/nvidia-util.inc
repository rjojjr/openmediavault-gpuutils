<?php

class OMVRpcServiceNvidiaUtil extends \OMV\Rpc\ServiceAbstract
{
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
        $gpuTempCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_TEMP_COMMAND",
            "nvidia-smi --query-gpu=index,temperature.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuTempCmd);
        $cmd->execute($output, $exitStatus);

        $temps = [];
        foreach ($output as $line) {
            list($id, $temp) = explode(", ", trim($line));
            $temps[] = ["gpuId" => intval($id), "gpuTemp" => intval($temp)];
        }
        return $temps;
    }

    public function getGpuUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuUtilCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_UTIL_COMMAND",
            "nvidia-smi --query-gpu=index,utilization.gpu --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuUtilCmd);
        $cmd->execute($output, $exitStatus);

        $utils = [];
        foreach ($output as $line) {
            list($id, $util) = explode(", ", trim($line));
            $utils[] = ["gpuId" => intval($id), "gpuUtil" => intval($util)];
        }
        return $utils;
    }

    public function getGpuPowerUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuPowerUtilCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_POWER_COMMAND",
            "nvidia-smi --query-gpu=index,power.draw,power.limit --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuPowerUtilCmd);
        $cmd->execute($output, $exitStatus);

        $powers = [];
        foreach ($output as $line) {
            list($id, $draw, $limit) = explode(", ", trim($line));
            $powers[] = ["gpuId" => intval($id), "gpuPowerDraw" => intval($draw), "gpuPowerLimit" => intval($limit)];
        }
        return $powers;
    }

    public function getGpuMemUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuMemUtilCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_MEM_UTIL_COMMAND",
            "nvidia-smi --query-gpu=index,memory.total,memory.free,memory.used --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuMemUtilCmd);
        $cmd->execute($output, $exitStatus);

        $mems = [];
        foreach ($output as $line) {
            list($id, $total, $free, $used) = explode(", ", trim($line));
            $mems[] = ["gpuId" => intval($id), "gpuMemTotal" => intval($total) * 1048576, "gpuMemAvailable" => intval($free) * 1048576, "gpuMem" => intval($used) * 1048576];
        }
        return $mems;
    }

    public function getGpuFanUtil($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $gpuFanUtilCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_FAN_UTIL_COMMAND",
            "nvidia-smi --query-gpu=index,fan.speed --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuFanUtilCmd);
        $cmd->execute($output, $exitStatus);

        $fans = [];
        foreach ($output as $line) {
            list($id, $fan) = explode(", ", trim($line));
            $fans[] = ["gpuId" => intval($id), "gpuFanUtil" => intval($fan)];
        }
        return $fans;
    }

    public function getAllGpuMetrics($param, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);

        $gpus = [];
        $gpuCountCmd = \OMV\Environment::get("OMV_NVIDIA_GPU_COUNT_COMMAND",
            "nvidia-smi --query-gpu=index --format=csv,noheader,nounits");
        $cmd = new \OMV\System\Process($gpuCountCmd);
        $cmd->execute($gpuIndexOutput, $exitStatus);

        foreach ($gpuIndexOutput as $line) {
            $id = intval(trim($line));

            // Get all GPU metrics
            $gpuTemps = array_column($this->getGpuTemp($param, $context), 'gpuTemp', 'gpuId');
            $gpuUtils = array_column($this->getGpuUtil($param, $context), 'gpuUtil', 'gpuId');
            $gpuPowers = array_column($this->getGpuPowerUtil($param, $context), 'gpuPowerDraw', 'gpuId');
            $gpuPowersLimit = array_column($this->getGpuPowerUtil($param, $context), 'gpuPowerLimit', 'gpuId');
            $gpuMems = array_column($this->getGpuMemUtil($param, $context), null, 'gpuId');
            $gpuFans = array_column($this->getGpuFanUtil($param, $context), 'gpuFanUtil', 'gpuId');

            // Gather all metrics into an array for this GPU
            $gpus[] = [
                'gpuId' => $id,
                'gpuTemp' => $gpuTemps[$id] ?? 0,
                'gpuUtil' => $gpuUtils[$id] ?? 0,
                'gpuPowerDraw' => $gpuPowers[$id] ?? 0,
                'gpuPowerLimit' => $gpuPowersLimit[$id] ?? 0,
                'gpuMemTotal' => $gpuMems[$id]['gpuMemTotal'] ?? 0,
                'gpuMemAvailable' => $gpuMems[$id]['gpuMemAvailable'] ?? 0,
                'gpuMemUsed' => $gpuMems[$id]['gpuMem'] ?? 0,
                'gpuFanUtil' => $gpuFans[$id] ?? 0,
            ];
        }
        return $gpus;
    }
}
