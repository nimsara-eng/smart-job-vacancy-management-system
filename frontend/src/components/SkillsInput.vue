<template>
  <div class="skills-input">
    <div class="si-tags">
      <span v-for="(skill, i) in modelValue" :key="i" class="si-tag">
        {{ skill }}
        <button type="button" @click="remove(i)">
          <svg width="10" height="10" viewBox="0 0 10 10"><path d="M2 2l6 6M8 2L2 8" stroke="currentColor" stroke-width="1.5" stroke-linecap="round"/></svg>
        </button>
      </span>
    </div>
    <div class="si-row">
      <input
        class="form-input si-input"
        v-model="current"
        :placeholder="placeholder"
        @keydown.enter.prevent="add"
        @keydown.comma.prevent="add"
      />
      <button type="button" class="btn btn-outline btn-sm" @click="add">Add</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

const props = defineProps<{
  modelValue: string[]
  placeholder?: string
}>()

const emit = defineEmits<{ 'update:modelValue': [value: string[]] }>()

const current = ref('')

function add() {
  const val = current.value.trim().replace(/,$/, '')
  if (val && !props.modelValue.includes(val)) {
    emit('update:modelValue', [...props.modelValue, val])
  }
  current.value = ''
}

function remove(i: number) {
  const updated = [...props.modelValue]
  updated.splice(i, 1)
  emit('update:modelValue', updated)
}
</script>

<style scoped>
.skills-input { display: flex; flex-direction: column; gap: 8px; }
.si-tags { display: flex; flex-wrap: wrap; gap: 6px; }
.si-tag {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  padding: 4px 10px;
  background: rgba(79,142,247,.12);
  border: 1px solid rgba(79,142,247,.25);
  border-radius: 100px;
  font-size: .8rem;
  color: var(--c-accent);
}
.si-tag button {
  background: none;
  border: none;
  cursor: pointer;
  color: currentColor;
  display: grid;
  place-items: center;
  opacity: .7;
  padding: 0;
}
.si-tag button:hover { opacity: 1; }
.si-row { display: flex; gap: 8px; }
.si-input { flex: 1; }
</style>