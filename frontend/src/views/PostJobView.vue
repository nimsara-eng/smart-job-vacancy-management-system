<template>
  <div class="page">
    <div class="container" style="max-width:720px">
      <div class="page-heading">
        <h1>Post a Job</h1>
        <p>Fill in the details below to list a new vacancy</p>
      </div>

      <Transition name="fade">
        <div v-if="success" class="alert alert-success">
          Job posted successfully!
          <RouterLink to="/manage-jobs" class="alert-link">View your listings →</RouterLink>
        </div>
      </Transition>
      <Transition name="fade">
        <div v-if="error" class="alert alert-error">{{ error }}</div>
      </Transition>

      <form class="post-form card" @submit.prevent="handleSubmit">
        <div class="form-section">
          <h2 class="form-section-title">Basic Info</h2>
          <div class="form-row">
            <div class="form-group">
              <label class="form-label">Job Title *</label>
              <input v-model="form.title" class="form-input" placeholder="e.g. Senior Software Engineer" required />
            </div>
            <div class="form-group">
              <label class="form-label">Company *</label>
              <input v-model="form.company" class="form-input" placeholder="Company name" required />
            </div>
          </div>
          <div class="form-row">
            <div class="form-group">
              <label class="form-label">Category *</label>
              <select v-model="form.category" class="form-select" required>
                <option value="" disabled>Select category</option>
                <option v-for="c in categories" :key="c">{{ c }}</option>
              </select>
            </div>
            <div class="form-group">
              <label class="form-label">Location *</label>
              <input v-model="form.location" class="form-input" placeholder="e.g. Colombo, Sri Lanka" required />
            </div>
          </div>
        </div>

        <div class="divider"/>

        <div class="form-section">
          <h2 class="form-section-title">Compensation & Type</h2>
          <div class="form-row">
            <div class="form-group">
              <label class="form-label">Monthly Salary (LKR) *</label>
              <input v-model.number="form.salary" class="form-input" type="number" min="0" step="1000" placeholder="e.g. 150000" required />
            </div>
            <div class="form-group">
              <label class="form-label">Work Type</label>
              <div class="toggle-row">
                <label class="toggle-label">
                  <input type="checkbox" v-model="form.isRemote" />
                  <span class="toggle-track">
                    <span class="toggle-thumb"/>
                  </span>
                  Remote Position
                </label>
              </div>
            </div>
          </div>
        </div>

        <div class="divider"/>

        <div class="form-section">
          <h2 class="form-section-title">Description & Skills</h2>
          <div class="form-group">
            <label class="form-label">Job Description *</label>
            <textarea v-model="form.description" class="form-textarea" placeholder="Describe the role, responsibilities, and what you're looking for…" required style="min-height:140px"/>
          </div>
          <div class="form-group">
            <label class="form-label">Required Skills</label>
            <SkillsInput v-model="form.requiredSkills" placeholder="e.g. Java, MySQL…" />
          </div>
        </div>

        <div class="form-footer">
          <RouterLink to="/manage-jobs" class="btn btn-ghost">Cancel</RouterLink>
          <button type="submit" class="btn btn-primary btn-lg" :disabled="loading">
            <span v-if="loading" class="spinner"/>
            {{ loading ? 'Posting…' : 'Post Job' }}
          </button>
        </div>
      </form>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive } from 'vue'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'
import SkillsInput from '../components/SkillsInput.vue'

const auth = useAuthStore()
const data = useDataStore()

const categories = ['Software', 'Hardware', 'Design', 'Marketing', 'Finance', 'HR', 'Sales', 'Operations', 'Other']

const form = reactive({
  title: '',
  company: auth.user?.name ?? '',
  category: '',
  location: '',
  salary: 0,
  isRemote: false,
  description: '',
  requiredSkills: [] as string[],
  isOpen: true,
})

const loading = ref(false)
const success = ref(false)
const error = ref('')

async function handleSubmit() {
  if (!form.requiredSkills.length) {
    error.value = 'Please add at least one required skill.'
    return
  }
  error.value = ''
  loading.value = true
  await new Promise(r => setTimeout(r, 500))
  data.addJob({ ...form })
  loading.value = false
  success.value = true
  // Reset
  Object.assign(form, { title: '', category: '', location: '', salary: 0, isRemote: false, description: '', requiredSkills: [] })
}
</script>

<style scoped>
.post-form { padding: 0; }
.form-section { padding: 28px 32px; }
.form-section-title { font-size: .95rem; margin-bottom: 20px; color: var(--c-text2); font-weight: 600; }
.form-row { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; margin-bottom: 16px; }
.form-row:last-child { margin-bottom: 0; }
.form-group { margin-bottom: 0; }

.toggle-row { display: flex; align-items: center; height: 42px; }
.toggle-label {
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  font-size: .9rem;
  color: var(--c-text2);
}
.toggle-label input { display: none; }
.toggle-track {
  width: 40px; height: 22px;
  background: var(--c-surface2);
  border: 1px solid var(--c-border);
  border-radius: 11px;
  position: relative;
  transition: background .2s, border-color .2s;
  flex-shrink: 0;
}
.toggle-thumb {
  position: absolute;
  top: 2px; left: 2px;
  width: 16px; height: 16px;
  border-radius: 50%;
  background: var(--c-text3);
  transition: transform .2s, background .2s;
}
.toggle-label input:checked ~ .toggle-track { background: rgba(79,142,247,.2); border-color: var(--c-accent); }
.toggle-label input:checked ~ .toggle-track .toggle-thumb { transform: translateX(18px); background: var(--c-accent); }

.form-footer { padding: 20px 32px; border-top: 1px solid var(--c-border); display: flex; justify-content: flex-end; gap: 12px; }

.alert-link { color: inherit; text-decoration: underline; margin-left: 8px; }

.spinner {
  width: 14px; height: 14px;
  border: 2px solid rgba(255,255,255,.3);
  border-top-color: #fff;
  border-radius: 50%;
  animation: spin .6s linear infinite;
  display: inline-block;
}
@keyframes spin { to { transform: rotate(360deg); } }
</style>