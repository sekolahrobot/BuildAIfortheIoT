#ifndef __MODEL_JSON_H__
#define __MODEL_JSON_H__

const char recognition_model_string_json[] = {"{\"NumModels\":1,\"ModelIndexes\":{\"0\":\"PIPELINE_RANK_1\"},\"ModelDescriptions\":[{\"Name\":\"PIPELINE_RANK_1\",\"ClassMaps\":{\"1\":\"BadSmell\",\"2\":\"Normal\",\"0\":\"Unknown\"},\"ModelType\":\"DecisionTreeEnsemble\",\"FeatureFunctions\":[\"Minimum\",\"GlobalPeaktoPeakofHighFrequency\",\"GlobalPeaktoPeak\",\"GlobalMinMaxSum\",\"PowerSpectrum\",\"PowerSpectrum\",\"InterquartileRange\",\"MFCC\",\"MFCC\"]}]}"};

int recognition_model_string_json_len = sizeof(recognition_model_string_json);

#endif /* __MODEL_JSON_H__ */
